#include "enemySpawnSystem.hpp"
#include "raylib.h"
#include "raymath.h"
#include "enemies/enemyFactory.hpp"
#include "defines/components/components.hpp"
#include "defines/components/entityComponents.hpp"
#include "utils/logs.h"
#include <algorithm>

const float MIN_SPAWN_DISTANCE = 150.0f;

void EnemySpawnSystem::init(const std::vector<EnemySpawnData>& enemies) {
    spawnStates.clear();
    for (const auto& enemy : enemies) {
        SpawnState state;
        state.data = enemy;
        state.spawnedCount = 0;
        state.timer = enemy.spawnInterval;
        state.initialSpawnRemaining = std::max(0, std::min(enemy.initialSpawnCount, enemy.count));
        spawnStates.push_back(state);
    }
}

void EnemySpawnSystem::update(entt::registry& registry, AssetManager& assetManager, GameDataManager& dataManager, float dt, float mapWidth, float mapHeight) {    
    // Ottieni la posizione del player una volta per evitare lookup ripetuti
    Vector2 playerPos = {0.0f, 0.0f};
    bool playerExists = false;
    
    auto playerView = registry.view<transform, is_player>();
    for (auto pEnt : playerView) {
        playerPos = playerView.get<transform>(pEnt).position;
        playerExists = true;
        break; // Ci aspettiamo un solo player
    }

    auto findValidSpawnPosition = [&](Vector2& outSpawnPos) -> bool {
        const int maxRetries = 20;

        for (int i = 0; i < maxRetries; ++i) {
            // Genera una posizione casuale all'interno dei limiti della mappa,
            // con un piccolo margine per evitare i bordi esatti.
            outSpawnPos = {
                static_cast<float>(GetRandomValue(50, static_cast<int>(mapWidth) - 50)),
                static_cast<float>(GetRandomValue(50, static_cast<int>(mapHeight) - 50))
            };

            bool validSpawnPos = true;

            // Controlla la distanza dal player
            if (playerExists && Vector2Distance(outSpawnPos, playerPos) < MIN_SPAWN_DISTANCE) {
                validSpawnPos = false;
            }

            // Controlla la distanza dagli altri nemici esistenti
            if (validSpawnPos) {
                auto existingEnemies = registry.view<transform, is_enemy>();
                for (auto existingEnemy : existingEnemies) {
                    auto& existingEnemyTransform = existingEnemies.get<transform>(existingEnemy);
                    if (Vector2Distance(outSpawnPos, existingEnemyTransform.position) < MIN_SPAWN_DISTANCE) {
                        validSpawnPos = false;
                        break; // Trovato un nemico troppo vicino, non serve controllare gli altri
                    }
                }
            }

            if (validSpawnPos) {
                return true;
            }
        }

        return false;
    };

    for (auto& state : spawnStates) {
        if (state.spawnedCount >= state.data.count) {
            continue;
        }

        const int remainingToCap = state.data.count - state.spawnedCount;

        // Prima svuotiamo lo spawn iniziale immediato in un unico tick.
        int spawnAttemptsThisTick = std::min(state.initialSpawnRemaining, remainingToCap);

        // Se non ci sono spawn immediati da fare, usa lo spawn periodico (1 alla volta).
        if (spawnAttemptsThisTick == 0) {
            state.timer -= dt;
            if (state.timer > 0.0f) {
                continue;
            }
            spawnAttemptsThisTick = 1;
        }

        EntityStaticData sData = dataManager.getEnemyStaticData(state.data.enemyType);

        for (int i = 0; i < spawnAttemptsThisTick && state.spawnedCount < state.data.count; ++i) {
            Vector2 spawnPos;
            if (findValidSpawnPosition(spawnPos)) {
                EnemyFactory::create(registry, assetManager, state.data, sData, spawnPos);
                state.spawnedCount++;
                if (state.initialSpawnRemaining > 0) {
                    state.initialSpawnRemaining--;
                }
            } else {
                APP_LOG("Warning: Could not find a valid spawn position for enemy type %s after %d retries. Skipping spawn for this cycle.", state.data.enemyType.c_str(), 20);
                break;
            }
        }

        // Lo spawn periodico resta sempre 1 alla volta ogni intervallo.
        if (state.initialSpawnRemaining == 0) {
            state.timer = state.data.spawnInterval;
        }
    }
}