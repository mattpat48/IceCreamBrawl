#include "enemySpawnSystem.hpp"
#include "raylib.h"
#include "raymath.h"
#include "enemies/enemyFactory.hpp"
#include "defines/components/components.hpp"
#include "defines/components/entityComponents.hpp"
#include "utils/logs.h"

const float MIN_SPAWN_DISTANCE = 150.0f;

void EnemySpawnSystem::init(const std::vector<EnemySpawnData>& enemies) {
    spawnStates.clear();
    for (const auto& enemy : enemies) {
        // Inizializza a 0 così il primo nemico spawna immediatamente
        spawnStates.push_back({enemy, 0, 0.0f});
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

    for (auto& state : spawnStates) {
        if (state.spawnedCount < state.data.count) {
            state.timer -= dt;
            if (state.timer <= 0.0f) {
                Vector2 spawnPos;
                bool validSpawnPos = false;
                int maxRetries = 20; // Numero massimo di tentativi per trovare una posizione valida

                for (int i = 0; i < maxRetries; ++i) {
                    // Genera una posizione casuale all'interno dei limiti della mappa,
                    // con un piccolo margine per evitare i bordi esatti.
                    spawnPos = {
                        static_cast<float>(GetRandomValue(50, static_cast<int>(mapWidth) - 50)),
                        static_cast<float>(GetRandomValue(50, static_cast<int>(mapHeight) - 50))
                    };
                    
                    validSpawnPos = true;

                    // Controlla la distanza dal player
                    if (playerExists) {
                        if (Vector2Distance(spawnPos, playerPos) < MIN_SPAWN_DISTANCE) {
                            validSpawnPos = false;
                        }
                    }

                    // Controlla la distanza dagli altri nemici esistenti
                    if (validSpawnPos) { // Solo se non è già invalida per il player
                        auto existingEnemies = registry.view<transform, is_enemy>();
                        for (auto existingEnemy : existingEnemies) {
                            auto& existingEnemyTransform = existingEnemies.get<transform>(existingEnemy);
                            if (Vector2Distance(spawnPos, existingEnemyTransform.position) < MIN_SPAWN_DISTANCE) {
                                validSpawnPos = false;
                                break; // Trovato un nemico troppo vicino, non serve controllare gli altri
                            }
                        }
                    }

                    if (validSpawnPos) {
                        break; // Trovata una posizione valida
                    }
                }
                
                if (validSpawnPos) {
                    EntityStaticData sData = dataManager.getEnemyStaticData(state.data.enemyType);
                    EnemyFactory::create(registry, assetManager, state.data, sData, spawnPos);
                    state.spawnedCount++;
                    state.timer = state.data.spawnInterval;
                } else {
                    APP_LOG("Warning: Could not find a valid spawn position for enemy type %s after %d retries. Skipping spawn for this cycle.", state.data.enemyType.c_str(), maxRetries);
                    // Se non troviamo una posizione valida, resetto il timer per ritentare al prossimo intervallo.
                    state.timer = state.data.spawnInterval;
                }
            }
        }
    }
}