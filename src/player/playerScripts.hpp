#pragma once

#include "defines/components/components.hpp"
#include "ui/controller/controller.hpp"
#include "ui/buttons/buttons.hpp"
#include "entities/entityStatus.hpp"
#include <entt/entt.hpp>

#include "raylib.h"
#include "raylib-cpp.hpp"

#include <iostream>

class playerScripts : public Script {
public:

    void onCreate() override {
        playerStatus.bind<entityStatus>(entity, *registry);
    }

    void onUpdate(float dt) override {
        playerStatus.instance->onUpdate(dt);
    }

    void onDraw() override {
        playerStatus.instance->onDraw();
    }

    virtual ~playerScripts() = default;

protected:
    script playerStatus;
};