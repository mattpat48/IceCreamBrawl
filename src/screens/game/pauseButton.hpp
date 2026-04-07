#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "engine/scripts/button/buttons.hpp"

class GameScreen;
class Engine;

class pauseButton : public buttonHandler {
public:
    pauseButton(GameScreen* gameScreen, Engine* engine)
        : buttonHandler(), gameScreen(gameScreen), engine(engine) {}

    virtual ~pauseButton();

    void buttonRelease() override;

private:
    GameScreen* gameScreen;
    Engine* engine;
};