#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "screenManager.hpp"

class Engine {
public:

    Engine(int width, int height, const char* title) : window(width, height, title) {
        SetTargetFPS(60); // Set the target frames per second
    }

    void setScreen(std::unique_ptr<Screen> screen) {
        screenManager.setScreen(std::move(screen));
    }

    void run() {
        while (!window.ShouldClose()) {

            float delta = GetFrameTime();

            screenManager.update(delta);

            window.BeginDrawing();
            window.ClearBackground(RAYWHITE);

            window.DrawFPS(10, 10);
            screenManager.draw();

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