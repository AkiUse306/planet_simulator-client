#include "planet.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>

PlanetState init_planet(int tile_count, float sea_level, unsigned int seed) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> lat_dist(-90.0, 90.0);
    std::uniform_real_distribution<double> lon_dist(-180.0, 180.0);
    std::uniform_real_distribution<float> elev_dist(-1.0f, 1.0f);
    std::uniform_real_distribution<float> temp_dist(-30.0f, 40.0f);
    std::uniform_real_distribution<float> hum_dist(0.0f, 1.0f);

    PlanetState state;
    state.tick = 0;
    state.tile_count = tile_count;
    state.sea_level = sea_level;
    state.tiles.reserve(tile_count);

    for (int i = 0; i < tile_count; ++i) {
        PlanetTile t;
        t.id = i;
        t.latitude = lat_dist(rng);
        t.longitude = lon_dist(rng);
        t.elevation = elev_dist(rng);
        t.ocean_depth = 0.0f;
        t.temperature = temp_dist(rng);
        t.humidity = hum_dist(rng);
        t.biome_id = 0;
        t.vegetation_density = 0.0f;
        t.civilization_id = -1;
        t.population = 0.0f;
        t.is_ocean = t.elevation < sea_level;
        state.tiles.push_back(t);
    }
    return state;
}

PlanetState load_state(const std::string &path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Could not open state file: " + path);

    PlanetState state;
    in >> state.tick >> state.tile_count >> state.sea_level;
    state.tiles.clear();
    state.tiles.reserve(state.tile_count);
    for (int i = 0; i < state.tile_count; ++i) {
        PlanetTile t;
        in >> t.id >> t.latitude >> t.longitude >> t.elevation >> t.ocean_depth >> t.temperature >> t.humidity >> t.biome_id >> t.vegetation_density >> t.civilization_id >> t.population >> t.is_ocean;
        state.tiles.push_back(t);
    }
    return state;
}

void save_state(const PlanetState &state, const std::string &path) {
    std::ofstream out(path);
    if (!out) throw std::runtime_error("Could not write state file: " + path);
    out << state.tick << " " << state.tile_count << " " << state.sea_level << "\n";
    for (const auto &t : state.tiles) {
        out << t.id << " " << t.latitude << " " << t.longitude << " " << t.elevation << " " << t.ocean_depth << " " << t.temperature << " " << t.humidity << " " << t.biome_id << " " << t.vegetation_density << " " << t.civilization_id << " " << t.population << " " << t.is_ocean << "\n";
    }
}

PlanetState step_tick(const PlanetState &state) {
    PlanetState next = state;
    next.tick += 1;
    for (auto &t : next.tiles) {
        t.elevation += (std::sin(t.latitude * 0.1) + std::cos(t.longitude * 0.1)) * 0.0005f;
        t.temperature += (std::sin(next.tick * 0.001 + t.latitude * 0.03) * 0.06f);
        t.humidity = std::max(0.0f, std::min(1.0f, t.humidity + static_cast<float>(std::sin(next.tick * 0.001 + t.longitude * 0.02) * 0.03f)));
        t.is_ocean = t.elevation < next.sea_level;
        t.ocean_depth = t.is_ocean ? std::max(0.0f, next.sea_level - t.elevation) : 0.0f;
        t.vegetation_density = std::max(0.0f, std::min(1.0f, t.vegetation_density + 0.005f - 0.003f * (t.is_ocean ? 0.2f : 1.0f)));
        t.population += t.civilization_id >= 0 ? t.population * 0.001f : 0.0f;
    }
    return next;
}

std::string snapshot_json(const PlanetState &state, int max_tiles) {
    std::ostringstream s;
    s << "{\"tick\": " << state.tick << ", \"tile_count\": " << state.tile_count << ", \"sea_level\": " << state.sea_level << ", \"tiles\": [";
    int count = 0;
    for (const auto &t : state.tiles) {
        if (count >= max_tiles) break;
        if (count > 0) s << ", ";
        s << "{\"id\": " << t.id << ", \"lat\": " << t.latitude << ", \"lon\": " << t.longitude << ", \"elevation\": " << t.elevation << ", \"temperature\": " << t.temperature << ", \"humidity\": " << t.humidity << ", \"biome_id\": " << t.biome_id << ", \"population\": " << t.population << "}";
        count++;
    }
    s << "]}";
    return s.str();
}
