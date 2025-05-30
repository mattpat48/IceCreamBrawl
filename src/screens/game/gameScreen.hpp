#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "screens/screen.hpp"
#include "engine/scripts/player/controller.hpp"
#include <entt/entt.hpp>
#include <memory>

class GameScreen : public Screen {
public:
    void load(entt::registry& globalRegistry) override {
        auto playerEntity = registry.create();;

        registry.emplace<sprite>(playerEntity, std::move(raylib::Texture2D("resources/sprites/walk.png")),
                                 SPRITES_HORIZONTAL_DIMENSION, SPRITES_VERTICAL_DIMENSION, Vector2{100.0f, 100.0f});
        registry.emplace<transform>(playerEntity, Vector2{100.0f, 100.0f}, Vector2{2.0f, 2.0f}, 0.0f);
        registry.emplace<velocity>(playerEntity, 0.0f, 0.0f);
        registry.emplace<animation>(playerEntity, 0, 0, 7, 0, 0.05f, 0.0f, false, 0);
        registry.emplace<script>(playerEntity).bind<Controller>();
    }

    void update(float delta) override {
        basicUpdate(delta);
    }

    void draw() override {
        basicDraw();
    }

    void unload(entt::registry& globalRegistry) override {
        //basicUnload();
    }
};
