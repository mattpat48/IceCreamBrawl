#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"
#include "entt/entt.hpp"
#include <iostream>

#include "defines/components/entityComponents.hpp"
#include "entities/game/combatEntities/combatEntityStatus.hpp"
#include "entities/game/entityInfos.hpp"

#include "utils/log.h"

class playerScripts : public Script {
public:

    void onUpdate(float dt) override {
        // TODO: remove comment once the right components are added
        //playerStatus.onUpdate(dt);
    }

    void onCreate() override {
        playerStatus.onCreate();
    }

    void onDraw() override {
        playerStatus.onDraw();
        // draw player coordinates in top-left corner for debugging
        auto t = getComponent<transform>();
        if (t) {
            DrawText(TextFormat("Player Pos: (%.1f, %.1f)", t->position.x, t->position.y), 10, 50, 40, WHITE);
        }
    }

    virtual ~playerScripts() = default;

protected:
    combatEntityStatus playerStatus;
};