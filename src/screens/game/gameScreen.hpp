#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "screens/screen.hpp"
#include "engine/scripts/player/controller.hpp"
#include "engine/scripts/player/buttons.hpp"
#include <entt/entt.hpp>
#include <memory>

class GameScreen : public Screen {
public:
    void load(entt::registry& globalRegistry) override {
        auto playerEntity = registry.create();

        registry.emplace<sprite>(playerEntity, std::move(raylib::Texture2D("resources/sprites/player/walk.png")),
                                 PLAYER_SPRITES_H_DIMENSION, PLAYER_SPRITES_V_DIMENSION, Vector2{100.0f, 100.0f});
        registry.emplace<transform>(playerEntity, Vector2{100.0f, 100.0f}, Vector2{2.0f, 2.0f}, 0.0f);
        registry.emplace<velocity>(playerEntity, 0.0f, 0.0f);
        registry.emplace<animation>(playerEntity, 0, 0, 7, 0, 0.05f, 0.0f, false, 0);
        //registry.emplace<script>(playerEntity).bind<touchController>();

        auto buttonEntity = registry.create();
        registry.emplace<sprite>(buttonEntity, std::move(raylib::Texture2D("resources/sprites/buttons/primary.png")),
                     BUTTON_SPRITES_H_DIMENSION, BUTTON_SPRITES_V_DIMENSION, Vector2{100.0f, 100.0f});
        registry.emplace<transform>(buttonEntity, Vector2{BUTTON_PRIMARY_X, BUTTON_PRIMARY_Y}, Vector2{BUTTON_PRIMARY_SCALE_X, BUTTON_PRIMARY_SCALE_Y}, 0.0f);
        registry.emplace<animation>(buttonEntity, BUTTON_PRIMARY_ANIMATION_START_FRAME,
                                                  BUTTON_PRIMARY_ANIMATION_END_FRAME,
                                                  BUTTON_PRIMARY_ANIMATION_ROW,
                                                  0, 
                                                  BUTTON_PRIMARY_ANIMATION_FRAME_TIME, 
                                                  0.0f, false, 0);
        registry.emplace<script>(buttonEntity).bind<buttonHandler>(playerEntity, registry);
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

/*

        constexpr float joystickBaseX = 45.0f;
        constexpr float joystickBaseY = 650.0f;
        constexpr float joystickBaseSize = 300.0f;
        constexpr float joystickBaseScale = 0.7f;

        constexpr float handleSize = 100.0f; // esempio: 100x100px handle
        constexpr float handleScale = 0.7f; // esempio: 0.7x scale

        // Calcola il centro del cerchio base
        float baseCenterX = joystickBaseX + (joystickBaseSize * joystickBaseScale) / 2.0f;
        float baseCenterY = joystickBaseY + (joystickBaseSize * joystickBaseScale) / 2.0f;

        // Posiziona la handle centrata rispetto al centro del cerchio
        float handleX = baseCenterX - (handleSize * handleScale) / 2.0f;
        float handleY = baseCenterY - (handleSize * handleScale) / 2.0f;

        auto joystickEntity = registry.create();
        registry.emplace<sprite>(joystickEntity, std::move(raylib::Texture2D("resources/sprites/controller/joystick.png")),
                     300, 300, Vector2{100.0f, 100.0f});
        registry.emplace<transform>(joystickEntity, Vector2{joystickBaseX, joystickBaseY}, Vector2{joystickBaseScale, joystickBaseScale}, 0.0f);
        registry.emplace<animation>(joystickEntity, 0, 0, 0, 0, 0.0f, 0.0f, false, 0);

        auto handleEntity = registry.create();
        registry.emplace<sprite>(handleEntity, std::move(raylib::Texture2D("resources/sprites/controller/handle.png")),
                     100, 100, Vector2{100.0f, 100.0f});
        registry.emplace<transform>(handleEntity, Vector2{handleX, handleY}, Vector2{handleScale, handleScale}, 0.0f);
        registry.emplace<velocity>(handleEntity, 0.0f, 0.0f);
        registry.emplace<animation>(handleEntity, 0, 0, 0, 0, 0.0f, 0.0f, false, 0);
        registry.emplace<script>(handleEntity).bind<touchController>(playerEntity, registry, Vector2{handleX, handleY}, joystickBaseScale * joystickBaseSize / 2.0f);

*/