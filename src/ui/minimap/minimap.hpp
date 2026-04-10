#pragma once

#include "raylib.h"
#include "raymath.h"
#include "engine/script.hpp"
#include "defines/components/components.hpp"
#include <entt/entt.hpp>

class minimap : public Script {
public:
    minimap(entt::entity player, float mWidth, float mHeight) 
        : playerEntity(player), mapWidth(mWidth), mapHeight(mHeight) {}

    void onCreate() override { }

    void onUpdate(float dt) override { }

    void onDraw() override;

private:
    entt::entity playerEntity;
    float mapWidth;
    float mapHeight;
};
