#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "screen.hpp"
#include "scripts/general/controller.hpp"
#include "scripts/general/buttons.hpp"
#include "scripts/entities/player/playerScripts.hpp"
#include "scripts/entities/player/primaryAttack.hpp"
#include "scripts/entities/enemy/enemyScripts.hpp"
#include "pauseButton.hpp"
#include <entt/entt.hpp>
#include <memory>

class GameScreen : public Screen {
public:
    bool paused = false;

    void load(entt::registry& globalRegistry) override {
        float scaleX = GetScreenWidth() / BASE_WIDTH;
        float scaleY = GetScreenHeight() / BASE_HEIGHT;
        float scale = std::min(scaleX, scaleY);

        
    }

    void update(float delta) override {
        if (!paused) basicUpdate(delta);
    }

    void draw() override {
        basicDraw();
    }

    void unload(entt::registry& globalRegistry) override {
        //basicUnload();
    }

    virtual ~GameScreen() = default;

protected:
    entt::entity playerEntity; // Reference to the player entity
    entt::entity enemyEntity; // Reference to the enemy entity
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