// pauseButton.cpp
#include "pauseButton.hpp"
#include "gameScreen.hpp"
#include "engine.hpp"

#include "pauseScreen.hpp"

void pauseButton::buttonRelease() {
    gameScreen->paused = !gameScreen->paused;
    engine->pushScreen(std::make_unique<PauseScreen>(gameScreen));
}