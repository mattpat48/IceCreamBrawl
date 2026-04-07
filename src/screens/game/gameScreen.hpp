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