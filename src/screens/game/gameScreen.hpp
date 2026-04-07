#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "screens/screen.hpp"
#include "engine/scripts/controller/controller.hpp"
#include "engine/scripts/button/buttons.hpp"
#include "engine/scripts/entities/player/playerScripts.hpp"
#include "engine/scripts/entities/enemy/enemyScripts.hpp"
#include "pauseButton.hpp"
#include <entt/entt.hpp>
#include <memory>

class GameScreen : public Screen {
public:
    bool paused = false;

    void load(entt::registry& globalRegistry) override;

    void update(float delta) override {
        if (!paused) basicUpdate(delta);
    }

    void draw() override {
        basicDraw();
    }

    void unload(entt::registry& globalRegistry) override;

    virtual ~GameScreen() = default;

protected:
    entt::entity playerEntity; // Reference to the player entity
};