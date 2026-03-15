from __future__ import annotations

import random
from typing import Dict, List

from .planet_tile import PlanetTile

class Planet:
    def __init__(self, tile_count: int = 16384, sea_level: float = 0.0):
        self.tile_count = tile_count
        self.sea_level = sea_level
        self.tiles: Dict[int, PlanetTile] = {}
        self.tick = 0
        self._init_tiles()

    def _init_tiles(self):
        for i in range(self.tile_count):
            lat = -90.0 + 180.0 * (i / max(1, self.tile_count - 1))
            lon = -180.0 + 360.0 * ((i * 137) % self.tile_count) / self.tile_count
            self.tiles[i] = PlanetTile(
                id=i,
                latitude=lat,
                longitude=lon,
                elevation=random.uniform(-1.0, 1.0),
                temperature=random.uniform(-30.0, 40.0),
                humidity=random.uniform(0.0, 1.0),
            )

    def update(self):
        self.tick += 1
        self._update_geology()
        self._update_ocean()
        self._update_climate()
        self._update_ecosystem()
        self._update_civilization()

    def _update_geology(self):
        # placeholder geology update
        for tile in self.tiles.values():
            tile.elevation += random.uniform(-0.001, 0.001)

    def _update_ocean(self):
        for tile in self.tiles.values():
            tile.update_ocean_state(self.sea_level)

    def _update_climate(self):
        for tile in self.tiles.values():
            tile.temperature += random.uniform(-0.1, 0.1)
            tile.humidity = max(0.0, min(1.0, tile.humidity + random.uniform(-0.01, 0.01)))

    def _update_ecosystem(self):
        for tile in self.tiles.values():
            if tile.biome_id == 0:
                tile.vegetation_density = max(0.0, min(1.0, tile.vegetation_density + 0.01))

    def _update_civilization(self):
        for tile in self.tiles.values():
            if tile.civilization_id >= 0:
                tile.population *= 1.0 + random.uniform(-0.001, 0.002)

    def snapshot(self) -> List[Dict]:
        return [tile.as_dict() for tile in self.tiles.values()]
