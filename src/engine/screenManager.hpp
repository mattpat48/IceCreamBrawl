#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "screens/screen.hpp"
#include <entt/entt.hpp>

#include <memory>

class ScreenManager {
public:
    Screen* currentScreen = nullptr;
    std::vector<std::unique_ptr<Screen>> screenStack;
    entt::registry globalRegistry;

    ~ScreenManager() {
        if (currentScreen) {
            currentScreen->unload(globalRegistry);
        }
        while (!screenStack.empty()) {
            screenStack.back()->unload(globalRegistry);
            screenStack.pop_back();
        }
    }

    void setScreen(std::unique_ptr<Screen> screen) {
        while (!screenStack.empty()) {
            screenStack.back()->unload(globalRegistry);
            screenStack.pop_back();
        }
        if (screen) {
            screen->load(globalRegistry);
            screenStack.push_back(std::move(screen));
            currentScreen = screenStack.back().get();
        } else {
            currentScreen = nullptr;
        }
    }

    void pushScreen(std::unique_ptr<Screen> screen) {
        if (screen) {
            screen->load(globalRegistry);
            screenStack.push_back(std::move(screen));
            currentScreen = screenStack.back().get();
        }
    }

    void popScreen() {
        if (!screenStack.empty()) {
            screenStack.back()->unload(globalRegistry);
            screenStack.pop_back();
            currentScreen = screenStack.empty() ? nullptr : screenStack.back().get();
        }
    }

    void update(float delta) {
        if (currentScreen) {
            currentScreen->update(delta);
        }
    }

    void draw() {
        for (auto& screen : screenStack) {
            screen->draw();
        }
    }
};