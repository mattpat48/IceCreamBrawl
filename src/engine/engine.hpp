#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "screenManager.hpp"
#include "assetManager.hpp"
#include "inputManager.hpp"
#include "movementManager.hpp"

class Engine {
public:

    Engine(int width, int height, const char* title) {
        SetConfigFlags(FLAG_WINDOW_HIGHDPI);
        InitWindow(width, height, title);
        SetTargetFPS(60); // Set the target frames per second
    }

    void setScreen(std::unique_ptr<Screen> screen) {
        screen->setEngine(this);
        // Pass the new screen's dispatcher to the input manager before setting the screen
        inputManager.setDispatcher(screen->getDispatcherPtr());
        movementManager.setRegistry(screen->getRegistryPtr());
        movementManager.setDispatcher(screen->getDispatcherPtr());
        screenManager.setScreen(std::move(screen));
    }

    void pushScreen(std::unique_ptr<Screen> screen) {
        screen->setEngine(this);
        // Pass the new screen's dispatcher to the input manager before pushing
        inputManager.setDispatcher(screen->getDispatcherPtr());
        movementManager.setRegistry(screen->getRegistryPtr());
        movementManager.setDispatcher(screen->getDispatcherPtr());
        screenManager.pushScreen(std::move(screen)); 
    }

    void popScreen() {
        screenManager.popScreen();
    }

    void run() {
        while (!WindowShouldClose()) {

            float delta = GetFrameTime();

            inputManager.update(delta);
            movementManager.update(delta);
            screenManager.update(delta);

            BeginDrawing();
            ClearBackground(LIGHTGRAY);

            DrawFPS(10, 10);
            DrawGrid(10, 10);
            screenManager.draw();

            EndDrawing();
        }
    }
    void close() {
        CloseWindow();
    }

    ScreenManager& getScreenManager() { return screenManager; }
    AssetManager& getAssetManager() { return assetManager; }
    InputManager& getInputManager() { return inputManager; }
    MovementManager& getMovementManager() { return movementManager; }

private:
    ScreenManager screenManager;
    AssetManager assetManager;
    InputManager inputManager;
    MovementManager movementManager;
};