#pragma once
#include "raylib.h"
#include "raylib-cpp.hpp"

#include "screens/screen.hpp"

#include <memory>

class ScreenManager {
public:
    std::unique_ptr<Screen> currentScreen;

    ~ScreenManager() {
        if (currentScreen) {
            currentScreen->unload();
        }
    }

    void setScreen(std::unique_ptr<Screen> screen) {
        if (currentScreen) {
            currentScreen->unload();
        }
        currentScreen = std::move(screen);
        if (currentScreen) {
            currentScreen->init();
        }
    }

    void update(float delta) {
        if (currentScreen) {
            currentScreen->update(delta);
        }
    }

    void draw() {
        if (currentScreen) {
            currentScreen->draw();
        }
    }
};