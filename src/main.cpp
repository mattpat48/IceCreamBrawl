#include "raylib.h"
#include "engine/engine.hpp"

#include "screens/game/gameScreen.hpp"

#include <iostream>

class Game : public Engine {
public:
    Game() : Engine(600, 800, "ICB Test Game") {
    }
};

int main() {

    Game engine;

    engine.setScreen(std::make_unique<GameScreen>());
    
    engine.run();

    engine.close();

    return 0;
}