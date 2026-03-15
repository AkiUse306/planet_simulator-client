from __future__ import annotations

from dataclasses import dataclass, field
from typing import List

MAX_SPECIES_PER_TILE = 16

@dataclass
class PlanetTile:
    id: int
    latitude: float
    longitude: float
    elevation: float = 0.0
    ocean_depth: float = 0.0
    temperature: float = 15.0
    humidity: float = 0.5
    pressure: float = 1013.25
    wind_x: float = 0.0
    wind_y: float = 0.0
    biome_id: int = 0
    vegetation_density: float = 0.0
    species_ids: List[int] = field(default_factory=list)
    civilization_id: int = -1
    population: float = 0.0
    is_ocean: bool = False

    def update_ocean_state(self, sea_level: float):
        self.is_ocean = self.elevation < sea_level
        if self.is_ocean:
            self.ocean_depth = max(sea_level - self.elevation, 0.0)
            self.temperature = min(self.temperature + 1.0, 30.0)
        else:
            self.ocean_depth = 0.0

    def as_dict(self):
        return {
            "id": self.id,
            "lat": self.latitude,
            "lon": self.longitude,
            "elevation": self.elevation,
            "ocean_depth": self.ocean_depth,
            "temperature": self.temperature,
            "humidity": self.humidity,
            "pressure": self.pressure,
            "biome_id": self.biome_id,
            "vegetation_density": self.vegetation_density,
            "population": self.population,
            "civilization_id": self.civilization_id,
        }
