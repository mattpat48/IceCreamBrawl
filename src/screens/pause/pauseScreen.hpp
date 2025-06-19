#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"
#include "screen.hpp"

#include "gameScreen.hpp"

class PauseScreen : public Screen {
public:
    GameScreen* gameScreen;

    PauseScreen(GameScreen* gs) : gameScreen(gs) {}

    void load(entt::registry& registry) override {}

    void unload(entt::registry& registry) override {}

    void update(float delta) override;

    void draw() override {
        // Disegna overlay trasparente e menu di pausa
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
        DrawText("PAUSA", GetScreenWidth()/2 - 75, GetScreenHeight()/2 - 20, 40, WHITE);
        // Disegna eventuali bottoni
    }
};