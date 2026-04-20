#pragma once

#include "raylib.h"

#include <entt/entt.hpp>

#include "screens/screen.hpp"
#include "defines/events.hpp"
#include "engine/gameplayRuntime.hpp"
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

    void onPlayerDeath(const PlayerDeathEvent& event);
    void onPlayerRespawn(const PlayerRespawnEvent& event);

protected:
    entt::entity playerEntity; // Reference to the player entity
    std::unique_ptr<GameplayRuntime> gameplayRuntime;

    void updateCamera(); // Logica per seguire il player
};