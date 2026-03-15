# C++ Planet Simulation Engine

## Build

```bash
cd backend/cpp-sim
mkdir -p build
cd build
cmake ..
cmake --build .
```

## Run

```bash
./sim init state.dat 4096 0.0 42
./sim tick state.dat state.dat
./sim snapshot state.dat 100
```
