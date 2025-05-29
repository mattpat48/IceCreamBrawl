#include "raylib.h"
#include "engine/engine.hpp"

#include "screens/game/gameScreen.hpp"

#include <iostream>

int main() {

    // Get the current device pixel ratio and dimensions
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    auto engine = Engine(0, 0, "ICB Test Game");
    engine.setScreen(std::make_unique<GameScreen>());

    engine.run();

    engine.close();

    return 0;
}