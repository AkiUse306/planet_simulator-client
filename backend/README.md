# Living Planet Simulator Backend

This backend provides a minimal simulation API for the Living Planet Simulator.

## Run locally

1. Install dependencies:

```bash
python -m venv .venv
source .venv/bin/activate
pip install -U pip
pip install fastapi uvicorn
```

2. Run API:

```bash
uvicorn backend.api.server:app --reload --port 8000
```

3. Build C++ engine and run API

```bash
cd backend/cpp-sim
mkdir -p build && cd build
cmake .. && cmake --build .
```

4. Run API

```bash
cd /workspaces/planet_simulator-client
source .venv/bin/activate
uvicorn backend.api.server:app --reload --port 8000
```

5. Use endpoints:
- `GET /health`
- `POST /tick`
- `GET /tiles`
- `GET /info`

6. Static client

Open `client/index.html` in a browser and use the buttons to call API endpoints (via local server reverse proxy or CORS).

