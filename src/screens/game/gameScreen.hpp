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

        auto playerEntity = registry.create();

        std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> playerTextures;
        playerTextures["idle"] = std::make_shared<raylib::Texture2D>("resources/sprites/player/idle.png");
        playerTextures["attack"] = std::make_shared<raylib::Texture2D>("resources/sprites/player/attack.png");
        playerTextures["death"] = std::make_shared<raylib::Texture2D>("resources/sprites/player/death.png");
        registry.emplace<sprite>(playerEntity,
                                 std::move(playerTextures), "idle",
                                 PLAYER_SPRITES_H_DIMENSION, PLAYER_SPRITES_V_DIMENSION);
        // position the player at the center of the screen
        registry.emplace<transform>(playerEntity,
                                    Vector2{GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f},
                                    Vector2{3.0f * scaleX, 3.0f * scaleY},
                                    0.0f);
        registry.emplace<velocity>(playerEntity, 0.0f, 0.0f);
        registry.emplace<animation>(playerEntity, 0, 0, 7, 1, 0.1f, 0.0f, true, 0);
        registry.emplace<script>(playerEntity).bind<playerScript>(playerEntity, registry);

        registry.emplace<hitFlash>(playerEntity);
        registry.emplace<status>(playerEntity, IDLE);
        registry.emplace<health>(playerEntity, 10.0f, 10.0f, 2.0f);
        registry.emplace<endurance>(playerEntity, 100.0f, 100.0f, 2.0f);
        registry.emplace<damage>(playerEntity, 1.0f, 1.0f, 0.0f, 1.0f);
        registry.emplace<attack>(playerEntity, 10.0f, 0.5f, false, 0.0f, 0.0f);

        this->playerEntity = playerEntity; // Store the player entity reference

        auto enemyEntity = registry.create();
        std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> enemyTextures;
        enemyTextures["idle"] = std::make_shared<raylib::Texture2D>("resources/sprites/enemy/idle.png");
        enemyTextures["death"] = std::make_shared<raylib::Texture2D>("resources/sprites/enemy/death.png");
        enemyTextures["attack"] = std::make_shared<raylib::Texture2D>("resources/sprites/enemy/attack.png");
        registry.emplace<sprite>(enemyEntity,
                                 std::move(enemyTextures), "idle",
                                 79, 69);
        registry.emplace<transform>(enemyEntity,
                                    Vector2{GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f - 200.0f*scaleY},
                                    Vector2{3.0f*scaleX, 3.0f*scaleY},
                                    0.0f);
        registry.emplace<velocity>(enemyEntity, 0.0f, 0.0f);
        registry.emplace<animation>(enemyEntity, 0, 0, 3, 0, 0.15f, 0.0f, true, 0);
        registry.emplace<script>(enemyEntity).bind<enemyScript>(enemyEntity, registry, playerEntity);

        registry.emplace<hitFlash>(enemyEntity);
        registry.emplace<status>(enemyEntity, IDLE);
        registry.emplace<health>(enemyEntity, 10.0f, 10.0f, 2.0f);
        registry.emplace<endurance>(enemyEntity, 100.0f, 100.0f, 2.0f);
        registry.emplace<damage>(enemyEntity, 1.0f, 1.0f);
        registry.emplace<attack>(enemyEntity, 10.0f, 0.5f, true, 0.0f, 0.0f);

        this->enemyEntity = enemyEntity; // Store the enemy entity reference

        auto attackButtonEntity = registry.create();
        std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> primaryTextures;
        primaryTextures["default"] = std::make_shared<raylib::Texture2D>("resources/sprites/buttons/primary.png");
        registry.emplace<sprite>(attackButtonEntity,
                                 std::move(primaryTextures), "default",
                                 BUTTON_SPRITES_H_DIMENSION, BUTTON_SPRITES_V_DIMENSION);
        registry.emplace<transform>(attackButtonEntity,
                                    Vector2{BUTTON_PRIMARY_X * scaleX, BUTTON_PRIMARY_Y * scaleY},
                                    Vector2{BUTTON_PRIMARY_SCALE_X * scaleX, BUTTON_PRIMARY_SCALE_Y * scaleY},
                                    0.0f);
        registry.emplace<animation>(attackButtonEntity,
                                    BUTTON_PRIMARY_ANIMATION_START_FRAME,
                                    BUTTON_PRIMARY_ANIMATION_END_FRAME,
                                    BUTTON_PRIMARY_ANIMATION_ROW,
                                    0,
                                    BUTTON_PRIMARY_ANIMATION_FRAME_TIME,
                                    0.0f,
                                    false,
                                    0);
        registry.emplace<script>(attackButtonEntity).bind<primaryAttack>(playerEntity, enemyEntity, registry);

        auto dodgeAreaEntity = registry.create();
        std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> dodgeAreaTextures;
        Image img = GenImageColor(GetScreenWidth() / 8.0f * 6.0f, GetScreenHeight() / 8.0f * 6.0f, WHITE);
        Texture2D tex = LoadTextureFromImage(img); 
        dodgeAreaTextures["default"] = std::make_shared<raylib::Texture2D>(tex);
        UnloadImage(img);
        registry.emplace<sprite>(dodgeAreaEntity,
                                 std::move(dodgeAreaTextures), "default",
                                 tex.width, tex.height);
        registry.emplace<transform>(dodgeAreaEntity,
                                    Vector2{float(GetScreenWidth() / 2.0f), float(GetScreenHeight() / 2.0f)},
                                    Vector2{1.0f, 1.0f},
                                    0.0f);
        registry.emplace<animation>(dodgeAreaEntity, 0, 0, 0, 0, 0.0f, 0.0f, false, 0);

        auto pauseButtonEntity = registry.create();
        std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> pauseTextures;
        pauseTextures["default"] = std::make_shared<raylib::Texture2D>("resources/sprites/buttons/pause.png");
        registry.emplace<sprite>(pauseButtonEntity,
                                 std::move(pauseTextures), "default",
                                 64, 64);
        registry.emplace<transform>(pauseButtonEntity,
                                    Vector2{500.0f * scaleX, 75.0f * scaleY},
                                    Vector2{1.0f * scaleX, 1.0f * scaleY},
                                    0.0f);
        registry.emplace<animation>(pauseButtonEntity,
                                    BUTTON_PRIMARY_ANIMATION_START_FRAME,
                                    BUTTON_PRIMARY_ANIMATION_END_FRAME,
                                    BUTTON_PRIMARY_ANIMATION_ROW,
                                    0,
                                    BUTTON_PRIMARY_ANIMATION_FRAME_TIME,
                                    0.0f,
                                    false,
                                    0);
        registry.emplace<script>(pauseButtonEntity).bind<pauseButton>(registry, this, getEngine());
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