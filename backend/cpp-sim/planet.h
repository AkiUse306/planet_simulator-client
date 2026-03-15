#pragma once

#include <string>
#include <vector>

struct PlanetTile {
    int id;
    double latitude;
    double longitude;
    float elevation;
    float ocean_depth;
    float temperature;
    float humidity;
    int biome_id;
    float vegetation_density;
    int civilization_id;
    float population;
    bool is_ocean;
};

struct PlanetState {
    int tick;
    int tile_count;
    float sea_level;
    std::vector<PlanetTile> tiles;
};

PlanetState init_planet(int tile_count, float sea_level, unsigned int seed);
PlanetState load_state(const std::string &path);
void save_state(const PlanetState &state, const std::string &path);
PlanetState step_tick(const PlanetState &state);
std::string snapshot_json(const PlanetState &state, int max_tiles);
