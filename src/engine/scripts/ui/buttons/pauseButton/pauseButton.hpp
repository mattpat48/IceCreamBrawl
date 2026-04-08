#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "engine/scripts/ui/buttons/buttons.hpp"
#include "screens/pause/pauseScreen.hpp"

class GameScreen;
class Engine;

class pauseButton : public buttonHandler {
public:
    pauseButton(GameScreen* gameScreen, Engine* engine)
        : buttonHandler(), gameScreen(gameScreen), engine(engine) {}

    void buttonRelease() override {
        gameScreen->paused = !gameScreen->paused;
        engine->pushScreen(std::make_unique<PauseScreen>(gameScreen));
    }

    virtual ~pauseButton() = default;

private:
    GameScreen* gameScreen;
    Engine* engine;
};