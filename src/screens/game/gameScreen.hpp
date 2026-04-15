#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "screens/screen.hpp"
#include "defines/events.hpp"
#include "ui/controller/controller.hpp"
#include "ui/buttons/buttons.hpp"
#include "player/playerScripts.hpp"
#include "enemies/enemyScripts.hpp"
#include "combat/combatSystem.hpp"
#include "combat/healthSystem.hpp"
#include "enemies/systems/enemyMovementSystem.hpp"
#include "enemies/systems/enemyAttackSystem.hpp"
#include "enemies/systems/enemySpawnSystem.hpp"
#include <entt/entt.hpp>
#include <memory>

class GameScreen : public Screen {
public:

    // Dimensioni della mappa in pixel
    float mapWidth = GetScreenWidth();
    float mapHeight = GetScreenHeight();

    void load(entt::registry& globalRegistry) override;

    void update(float delta) override;

    void draw() override;

    void unload(entt::registry& globalRegistry) override;

    virtual ~GameScreen() = default;

    //void onPauseToggle(const PauseToggleEvent& event);
    void onPlayerDeath(const PlayerDeathEvent& event);
    void onPlayerRespawn(const PlayerRespawnEvent& event);

protected:
    entt::entity playerEntity; // Reference to the player entity
    CombatSystem combatManager;
    HealthSystem healthManager;

    void rebindPlayer(entt::entity newPlayer);

    EnemySpawnSystem enemySpawnSystem;
    EnemyMovementSystem enemyMovementSystem;
    EnemyAttackSystem enemyAttackSystem;

    void updateCamera(); // Logica per seguire il player
};