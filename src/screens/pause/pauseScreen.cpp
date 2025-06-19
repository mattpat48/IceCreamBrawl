#include "pauseScreen.hpp"
#include "engine.hpp"

void PauseScreen::update(float delta) {
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        gameScreen->paused = false;
        engine->popScreen();
    }
}