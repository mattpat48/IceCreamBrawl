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

    void init(const LevelData& levelData, AssetManager& assetManager, GameDataManager& dataManager);
    void update(entt::registry& registry, float dt);

private:
    std::vector<SpawnState> spawnStates;
    AssetManager* assetManager = nullptr;
    GameDataManager* dataManager = nullptr;
    float mapWidth = 0.0f;
    float mapHeight = 0.0f;
};