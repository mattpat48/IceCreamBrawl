#pragma once

#include "defines/components/components.hpp"
#include "ui/controller/controller.hpp"
#include "ui/buttons/buttons.hpp"
#include "player/playerStatus.hpp"
#include <entt/entt.hpp>

#include "raylib.h"
#include "raylib-cpp.hpp"

#include <iostream>

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