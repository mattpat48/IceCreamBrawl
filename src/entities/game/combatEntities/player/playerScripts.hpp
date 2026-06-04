#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"
#include "entt/entt.hpp"
#include <iostream>

#include "defines/components/entityComponents.hpp"
#include "entities/game/combatEntities/combatEntityStatus.hpp"
#include "entities/entityInfos.hpp"

#include "utils/log.h"

class playerScripts : public Script {
public:

    void onUpdate(float dt) override {
        // TODO: remove comment once the right components are added
        //playerStatus.onUpdate(dt);
    }

    void onCreate() override {
        //playerStatus.onCreate();
    }

    void onDraw() override {
        //playerStatus.onDraw();
        // draw player coordinates in top-left corner for debugging
        auto t = getComponent<transform>();
        if (t) {
            DrawText(TextFormat("Player Pos: (%.1f, %.1f)", t->position.x, t->position.y), 10, 50, 40, WHITE);
        }
        drawGridOutline();
    }

    void drawGridOutline() {
        const EntityDatabase& db = EntityDatabase::getInstance();
        int rows = GridVariables::GRID_ROWS;
        int cols = GridVariables::GRID_COLUMNS;
    
        // spacing calcolato dalle posizioni dei centri (assume >1 riga/colonna)
        float spacingX = (cols > 1)
            ? fabs(db.getCellCenter(Rows::FIRST, Columns::SECOND).x - db.getCellCenter(Rows::FIRST, Columns::FIRST).x)
            : 100.0f;
        float spacingY = (rows > 1)
            ? fabs(db.getCellCenter(Rows::SECOND, Columns::FIRST).y - db.getCellCenter(Rows::FIRST, Columns::FIRST).y)
            : 100.0f;
    
        float halfW = spacingX * 0.5f;
        float halfH = spacingY * 0.5f;
    
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                Vector2 center = db.getCellCenter(static_cast<Rows>(r), static_cast<Columns>(c));
                Rectangle rect = { center.x - halfW, center.y - halfH, spacingX, spacingY };
                DrawRectangleLinesEx(rect, 4, RED);
            }
        }
    }

    virtual ~playerScripts() = default;

protected:
    combatEntityStatus playerStatus;
};