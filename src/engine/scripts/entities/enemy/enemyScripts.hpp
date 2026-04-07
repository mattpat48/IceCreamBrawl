#pragma once

#include "components/components.hpp"
#include "scripts/entities/entityStatus.hpp"
#include "enemyAttack.hpp"

class enemyScript : public Script {
public:

    void onUpdate(float dt) override {
    }

    void onCreate() override {
        enemyStatus.bind<entityStatus>(entity, *registry);
        primaryAttack.bind<enemyAttack>(entity, *registry);
    }

    void onDraw() override {
    }

    virtual ~enemyScript() = default;

protected:
    script enemyStatus;
    script primaryAttack;
};