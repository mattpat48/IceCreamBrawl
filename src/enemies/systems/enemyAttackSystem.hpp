#pragma once

#include "defines/components/components.hpp"
#include "defines/components/entityComponents.hpp"
#include <entt/entt.hpp>

class EnemyAttackSystem {
public:
    EnemyAttackSystem() = default;
    ~EnemyAttackSystem() = default;
    
    void update(entt::registry& registry, entt::entity playerEntity, float dt);
};