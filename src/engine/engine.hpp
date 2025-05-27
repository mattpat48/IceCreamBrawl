#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "screenManager.hpp"

class Engine {
public:

    void init(int width, int height, const char* title) {
        window.Init(width, height, title);
        SetTargetFPS(60); // Set the target frames per second
    }


    void run() {
        while (!window.ShouldClose()) {
            float delta = GetFrameTime();
            //Update(delta);

            window.BeginDrawing();
            window.ClearBackground(RAYWHITE);

            window.DrawFPS(10, 10);

            window.EndDrawing();
        }
    }
    void close() {
        window.Close();
    }

private:
    ScreenManager screenManager;
    raylib::Window window;
};