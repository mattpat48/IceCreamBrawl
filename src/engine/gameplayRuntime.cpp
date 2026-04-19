#include "engine/gameplayRuntime.hpp"
#include "player/playerFactory.hpp"

void GameplayRuntime::initialize(const LevelData& levelData) {
    enemySpawnSystem.init(levelData.enemies);
}

void GameplayRuntime::update(entt::registry& registry, float dt, entt::entity playerEntity, float mapWidth, float mapHeight) {
    combatSystem.update(registry, dt);
    healthSystem.update(registry, dt);

    enemySpawnSystem.update(registry, assetManager, dataManager, dt, mapWidth, mapHeight);
    enemyMovementSystem.update(registry, playerEntity, dt);
    enemyAttackSystem.update(registry, playerEntity, dt);
}

void GameplayRuntime::updatePlayerEntity(entt::entity newPlayer) {
    enemyMovementSystem.updatePlayerEntity(newPlayer);
    enemyAttackSystem.updatePlayerEntity(newPlayer);
}

void GameplayRuntime::handlePlayerDeath(entt::registry& registry) {
    healthSystem.resetEntities(registry);
}

entt::entity GameplayRuntime::respawnPlayer(entt::registry& registry, entt::entity currentPlayer) {
    if (registry.valid(currentPlayer)) {
        registry.destroy(currentPlayer);
    }

    PlayerSaveData pData = dataManager.getPlayerData();
    auto pStaticData = dataManager.getPlayerStaticData();
    entt::entity newPlayerEntity = PlayerFactory::create(registry, assetManager, pData, pStaticData);

    updatePlayerEntity(newPlayerEntity);
    return newPlayerEntity;
}
