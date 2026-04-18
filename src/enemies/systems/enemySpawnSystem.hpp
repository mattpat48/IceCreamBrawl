#pragma once
#include <entt/entt.hpp>
#include <vector>
#include "engine/gameDataManager.hpp"
#include "engine/assetManager.hpp"

struct SpawnState {
    EnemySpawnData data;
    int spawnedCount = 0;
    float timer = 0.0f;
    int initialSpawnRemaining = 0;
};

class EnemySpawnSystem {
public:
    EnemySpawnSystem() = default;
    ~EnemySpawnSystem() = default;

    void init(const std::vector<EnemySpawnData>& enemies);
    void update(entt::registry& registry, AssetManager& assetManager, GameDataManager& dataManager, float dt, float mapWidth, float mapHeight);

private:
    std::vector<SpawnState> spawnStates;
};