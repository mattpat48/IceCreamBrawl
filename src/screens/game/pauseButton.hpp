#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "buttons.hpp"

class GameScreen;
class Engine;

class pauseButton : public buttonHandler {
public:
    pauseButton(entt::registry& registry, GameScreen* gameScreen, Engine* engine)
        : buttonHandler(registry), gameScreen(gameScreen), engine(engine) {}

    void buttonRelease() override;

private:
    GameScreen* gameScreen;
    Engine* engine;
};