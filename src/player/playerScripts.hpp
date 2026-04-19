#pragma once

#include "defines/components/components.hpp"
#include "player/playerStatus.hpp"

class playerScripts : public Script {
public:

    void onCreate() override {
        status.bind<playerStatus>(entity, *registry);
    }

    void onUpdate(float dt) override {
        status.instance->onUpdate(dt);
    }

    void onDraw() override {
        status.instance->onDraw();
    }

    virtual ~playerScripts() = default;

protected:
    script status;
};