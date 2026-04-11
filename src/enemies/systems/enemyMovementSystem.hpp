#pragma once
#include <entt/entt.hpp>

class EnemyMovementSystem {
public:
    EnemyMovementSystem() = default;
    ~EnemyMovementSystem() = default;
    void update(entt::registry& registry, entt::entity playerEntity, float dt);
};