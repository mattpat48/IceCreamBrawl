#include "enemySpawnSystem.hpp"
#include "raylib.h"
#include "enemies/enemyFactory.hpp"

void EnemySpawnSystem::init(const std::vector<EnemySpawnData>& enemies) {
    spawnStates.clear();
    for (const auto& enemy : enemies) {
        // Inizializza a 0 così il primo nemico spawna immediatamente
        spawnStates.push_back({enemy, 0, 0.0f});
    }
}

void EnemySpawnSystem::update(entt::registry& registry, AssetManager& assetManager, GameDataManager& dataManager, float dt, float mapWidth, float mapHeight) {
    for (auto& state : spawnStates) {
        if (state.spawnedCount < state.data.count) {
            state.timer -= dt;
            if (state.timer <= 0.0f) {
                Vector2 spawnPos = {
                    static_cast<float>(GetRandomValue(100, static_cast<int>(mapWidth) - 100)),
                    static_cast<float>(GetRandomValue(100, static_cast<int>(mapHeight) - 100))
                };
                
                EntityStaticData sData = dataManager.getEnemyStaticData(state.data.enemyType);
                EnemyFactory::create(registry, assetManager, state.data, sData, spawnPos);
                state.spawnedCount++;
                state.timer = state.data.spawnInterval;
            }
        }
    }
}