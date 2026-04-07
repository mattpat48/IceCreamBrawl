#pragma once

#include "components/components.hpp"
#include "scripts/general/controller.hpp"
#include "scripts/general/buttons.hpp"
#include "scripts/entities/entityStatus.hpp"
#include <entt/entt.hpp>

#include "raylib.h"
#include "raylib-cpp.hpp"

#include <iostream>

class playerScripts : public Script {
public:

    void onUpdate(float dt) override {
        //dirController.instance->onUpdate(dt);
    }

    void onCreate() override {
        playerStatus.bind<entityStatus>(entity, *registry);
    }

    void onDraw() override {
    }

    virtual ~playerScripts() = default;

protected:
    script playerStatus;
};