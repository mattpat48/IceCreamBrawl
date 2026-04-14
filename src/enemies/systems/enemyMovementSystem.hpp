#pragma once
#include <entt/entt.hpp>

class EnemyMovementSystem {
public:
    EnemyMovementSystem() = default;
    void update(entt::registry& registry, entt::entity playerEntity, float dt);
    void updatePlayerEntity(entt::entity newPlayer) { playerEntity = newPlayer; }
private:
    entt::entity playerEntity;
};