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
        //playerStatus.onUpdate(dt);
        auto playerPosition = getComponent<gridInfo>();
        auto playerTransform = getComponent<transform>();
        if (playerPosition && playerTransform) {
            ICB_LOGI("Updating player position to cell center of (%d, %d)", int(playerPosition->row), int(playerPosition->column));
            Vector2 cellCenter = EntityDatabase::getInstance().getCellCenter(playerPosition->row, playerPosition->column);
            playerTransform->position = cellCenter;
        }
    }

    void onCreate() override {
        //playerStatus.onCreate();
    }

    void onDraw() override {
        //playerStatus.onDraw();
    }

    virtual ~playerScripts() = default;

protected:
    combatEntityStatus playerStatus;
};