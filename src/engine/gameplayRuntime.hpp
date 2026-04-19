#pragma once

#include <entt/entt.hpp>

#include "engine/assetManager.hpp"
#include "engine/gameDataManager.hpp"
#include "combat/combatSystem.hpp"
#include "combat/healthSystem.hpp"
#include "enemies/systems/enemySpawnSystem.hpp"
#include "enemies/systems/enemyMovementSystem.hpp"
#include "enemies/systems/enemyAttackSystem.hpp"

class GameplayRuntime {
public:
    GameplayRuntime(AssetManager& assetManager, GameDataManager& dataManager)
        : assetManager(assetManager), dataManager(dataManager) {
    }

    void initialize(const LevelData& levelData);

    void update(entt::registry& registry, float dt, entt::entity playerEntity, float mapWidth, float mapHeight);

    void updatePlayerEntity(entt::entity newPlayer);
    void handlePlayerDeath(entt::registry& registry);
    entt::entity respawnPlayer(entt::registry& registry, entt::entity currentPlayer);

    CombatSystem& getCombatSystem() { return combatSystem; }

private:
    AssetManager& assetManager;
    GameDataManager& dataManager;

    CombatSystem combatSystem;
    HealthSystem healthSystem;
    EnemySpawnSystem enemySpawnSystem;
    EnemyMovementSystem enemyMovementSystem;
    EnemyAttackSystem enemyAttackSystem;
};
