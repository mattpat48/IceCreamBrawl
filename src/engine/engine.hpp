#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "screenManager.hpp"

class Engine {
public:

    Engine(int width, int height, const char* title) {
        SetConfigFlags(FLAG_WINDOW_HIGHDPI);
        InitWindow(width, height, title);
        SetTargetFPS(60); // Set the target frames per second
        std::cout << "Screen: " << GetScreenWidth() << " x " << GetScreenHeight() << std::endl;
    }

    void setScreen(std::unique_ptr<Screen> screen) {
        screen->setEngine(this);
        screenManager.setScreen(std::move(screen));
    }

    void pushScreen(std::unique_ptr<Screen> screen) {
        screen->setEngine(this);
        screenManager.pushScreen(std::move(screen)); 
    }

    void popScreen() {
        screenManager.popScreen();
    }

    void run() {
        while (!WindowShouldClose()) {

            float delta = GetFrameTime();

            screenManager.update(delta);

            BeginDrawing();
            ClearBackground(LIGHTGRAY);

            DrawFPS(10, 10);
            screenManager.draw();

            EndDrawing();
        }
    }
    void close() {
        CloseWindow();
    }

private:
    ScreenManager screenManager;
};