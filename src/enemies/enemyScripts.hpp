#pragma once

#include "defines/components/components.hpp"
#include "enemies/enemyStatus.hpp"

class enemyScripts : public Script {
public:

    void onCreate() override {
        status.bind<enemyStatus>(entity, *registry);
    }

    void onUpdate(float dt) override {
        status.instance->onUpdate(dt);
    }

    void onDraw() override {
        status.instance->onDraw();
    }

    virtual ~enemyScripts() = default;

protected:
    script status;
};