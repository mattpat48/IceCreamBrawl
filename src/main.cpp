#include "raylib.h"
#include "engine/engine.hpp"

int main() {
    auto engine = Engine();

    // Get the current device pixel ratio and dimensions
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    engine.init(width, height, "ICB Test Engine");
    engine.run();
    engine.close();

    return 0;
}