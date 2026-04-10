#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "screens/screen.hpp"
#include "ui/controller/controller.hpp"
#include "ui/buttons/buttons.hpp"
#include "entities/player/playerScripts.hpp"
#include "entities/enemy/enemyScripts.hpp"
#include "combat/combatSystem.hpp"
#include "combat/healthSystem.hpp"
#include <entt/entt.hpp>
#include <memory>

class GameScreen : public Screen {
public:
    bool paused = false;

    // Dimensioni della mappa in pixel
    float mapWidth = 2000.0f;
    float mapHeight = 3000.0f;

    void load(entt::registry& globalRegistry) override;

    void update(float delta) override {
        if (!paused) {
            basicUpdate(delta);
            updateCamera();
            combatManager.update(registry, delta);
            healthManager.update(registry, delta);
        }
    }

    void draw() override {
        basicDraw();
    }

    void unload(entt::registry& globalRegistry) override;

    virtual ~GameScreen() = default;

protected:
    entt::entity playerEntity; // Reference to the player entity
    CombatSystem combatManager;
    HealthSystem healthManager;

    void updateCamera(); // Logica per seguire il player
};