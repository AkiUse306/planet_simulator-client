import json
import subprocess
from pathlib import Path

from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import JSONResponse

from ..simulation.planet import Planet

app = FastAPI(title="Living Planet Simulator API")
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# C++ simulation integration
ROOT = Path(__file__).resolve().parents[1]
SIM_BINARY = ROOT / "cpp-sim" / "build" / "sim"
STATE_FILE = ROOT / "cpp-sim" / "state.dat"
CPP_ENABLED = SIM_BINARY.exists()

planet = Planet(tile_count=2000, sea_level=0.0)


def _ensure_cpp_state():
    if not CPP_ENABLED:
        return False
    if not STATE_FILE.exists():
        subprocess.run([str(SIM_BINARY), "init", str(STATE_FILE), "4096", "0.0", "42"], check=True)
    return True


def _cpp_tick():
    _ensure_cpp_state()
    if not STATE_FILE.exists():
        raise RuntimeError("C++ state file missing")
    subprocess.run([str(SIM_BINARY), "tick", str(STATE_FILE), str(STATE_FILE)], check=True)
    out = subprocess.run([str(SIM_BINARY), "snapshot", str(STATE_FILE), "200"], capture_output=True, text=True, check=True)
    return json.loads(out.stdout)


@app.on_event("startup")
def startup_event():
    global CPP_ENABLED
    CPP_ENABLED = SIM_BINARY.exists()
    if CPP_ENABLED:
        _ensure_cpp_state()


@app.get("/health")
def health_check():
    if CPP_ENABLED:
        _ensure_cpp_state()
        state = _cpp_tick()  # this increments state, but for health we just check it
        return {"status": "ok", "tick": state.get("tick", 0), "engine": "cpp"}
    return {"status": "ok", "tick": planet.tick, "engine": "python"}


@app.post("/tick")
def tick():
    if CPP_ENABLED:
        _cpp_tick()
        out = subprocess.run([str(SIM_BINARY), "snapshot", str(STATE_FILE), "0"], capture_output=True, text=True, check=True)
        state = json.loads(out.stdout)
        return {"tick": state.get("tick", 0), "engine": "cpp"}

    planet.update()
    return {"tick": planet.tick, "engine": "python"}


@app.get("/tiles")
def get_tiles():
    if CPP_ENABLED:
        _ensure_cpp_state()
        out = subprocess.run([str(SIM_BINARY), "snapshot", str(STATE_FILE), "200"], capture_output=True, text=True, check=True)
        return JSONResponse(content=json.loads(out.stdout))

    return JSONResponse(content={"tick": planet.tick, "tile_count": len(planet.tiles), "tiles": planet.snapshot()[:200]})


@app.get("/info")
def info():
    return {"cpp_enabled": CPP_ENABLED, "sim_binary": str(SIM_BINARY), "state_file": str(STATE_FILE)}
