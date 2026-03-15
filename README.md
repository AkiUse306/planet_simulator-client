# Living Planet Simulator Client

A minimal platform scaffold for a planetary simulation engine and client.

## Components

- `ARCHITECTURE.md` — full engineering architecture overview
- `backend/` — simulation engine and API (Python+CPP)
- `backend/cpp-sim/` — C++ simulation core
- `client/` — React viewer UI

## Essentials

- Root `package.json` for monorepo scripts
- Root `.gitignore` for Python, C++, Node artifacts
- `backend/api/server.py` for HTTP simulation API
- `client/src/App.jsx` for React dashboard

## Quick Start

```bash
cd /workspaces/planet_simulator-client
npm run install:all
npm run backend:init-cpp
npm run backend:run
```

In another terminal:

```bash
cd /workspaces/planet_simulator-client
npm run client:dev
```

Open the client at `http://localhost:5173` (or printed port).

## API Endpoints

- `GET /health`
- `POST /tick`
- `GET /tiles`
- `GET /info`

## C++ engine

Init and run the C++ sim binary manually:

```bash
cd backend/cpp-sim
mkdir -p build && cd build
cmake .. && cmake --build .
./sim init state.dat 4096 0.0 42
./sim tick state.dat state.dat
./sim snapshot state.dat 100
```

### Run the Node client

```bash
cd /workspaces/planet_simulator-client/client
npm install
npm run dev
```

Open the client app at `http://localhost:5173` (or printed port), and use the dashboard controls.

### Build C++ engine

```bash
cd backend/cpp-sim
mkdir -p build && cd build
cmake .. && cmake --build .
```

### Run the API

```bash
cd /workspaces/planet_simulator-client
source .venv/bin/activate
uvicorn backend.api.server:app --reload --port 8000
```

