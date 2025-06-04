#pragma once

#include "components/components.hpp"

class enemyAttack : public Script {
public:
    enemyAttack(entt::entity entity, entt::entity player, entt::registry& registry) {
        this->entity = entity;
        this->playerEntity = player;
        this->registry = &registry;
    }

    void onUpdate(float dt) override {
        auto atk = getComponent<attack>;
        auto sta = getComponent<status>;

        auto psta = registry->try_get<status>(playerEntity);
        auto phea = registry->try_get<health>(playerEntity);

        if (sta.isIdle() && atk->canAttack() && !psta->isDead()) {
            
        }
    }

    void onDraw() override {

    }

protected:
    entt::entity playerEntity;
};