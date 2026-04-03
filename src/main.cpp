#include "raylib.h"
#include "engine/engine.hpp"

#include "screens/game/gameScreen.hpp"

#include <iostream>

class Game : public Engine {
public:
    Game(int width, int height) : Engine(width, height, "ICB Test Game") {
    }
};

int main() {

    auto width = GetScreenWidth();
    auto height = GetScreenHeight();

    std :: cout << "Initializing game with resolution: " << width << " x " << height << std::endl;

    Game engine(width, height);

    engine.setScreen(std::make_unique<GameScreen>());
    
    engine.run();

    engine.close();

    return 0;
}