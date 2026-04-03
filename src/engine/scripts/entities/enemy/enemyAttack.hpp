#pragma once

#include "components/components.hpp"

#include <iostream>

class enemyAttack : public Script {
public:
    enemyAttack(entt::entity entity, entt::entity player, entt::registry& registry) {
        this->entity = entity;
        this->playerEntity = player;
        this->registry = &registry;
    }

    void onUpdate(float dt) override {
    }

    void onDraw() override {

    }

    virtual ~enemyAttack() = default;

protected:
    entt::entity playerEntity;
};