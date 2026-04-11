#pragma once

#include "defines/components/components.hpp"
#include "entities/entityStatus.hpp"

class enemyScripts : public Script {
public:

    void onCreate() override {
        enemyStatus.bind<entityStatus>(entity, *registry);
    }

    void onUpdate(float dt) override {
        enemyStatus.instance->onUpdate(dt);
    }

    void onDraw() override {
        enemyStatus.instance->onDraw();
    }

    virtual ~enemyScripts() = default;

protected:
    script enemyStatus;
};