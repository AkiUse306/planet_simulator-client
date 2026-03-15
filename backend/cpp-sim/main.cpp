#include "planet.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: sim <init|tick|snapshot> [args]\n";
        return 1;
    }
    std::string cmd = argv[1];
    try {
        if (cmd == "init") {
            if (argc != 6) {
                std::cerr << "Usage: sim init <state_file> <tile_count> <sea_level> <seed>\n";
                return 1;
            }
            std::string state_file = argv[2];
            int tile_count = std::stoi(argv[3]);
            float sea_level = std::stof(argv[4]);
            unsigned int seed = std::stoul(argv[5]);
            auto state = init_planet(tile_count, sea_level, seed);
            save_state(state, state_file);
            std::cout << "{\"status\": \"initialized\", \"tile_count\": " << tile_count << ", \"tick\": 0}" << std::endl;
            return 0;
        } else if (cmd == "tick") {
            if (argc != 4) {
                std::cerr << "Usage: sim tick <state_file> <out_file>\n";
                return 1;
            }
            std::string state_file = argv[2];
            std::string out_file = argv[3];
            auto state = load_state(state_file);
            auto next = step_tick(state);
            save_state(next, out_file);
            std::cout << "{\"status\": \"ok\", \"tick\": " << next.tick << "}" << std::endl;
            return 0;
        } else if (cmd == "snapshot") {
            if (argc != 4) {
                std::cerr << "Usage: sim snapshot <state_file> <max_tiles>\n";
                return 1;
            }
            std::string state_file = argv[2];
            int max_tiles = std::stoi(argv[3]);
            auto state = load_state(state_file);
            std::cout << snapshot_json(state, max_tiles) << std::endl;
            return 0;
        }
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 2;
    }
    std::cerr << "Unknown command: " << cmd << "\n";
    return 1;
}
