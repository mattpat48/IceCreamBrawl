#pragma once

#include "screens/screen.hpp"
#include <entt/entt.hpp>

#include <memory>

class ScreenManager {
public:
    Screen* currentScreen = nullptr;
    std::vector<std::unique_ptr<Screen>> screenStack;
    entt::registry globalRegistry;

    void setScreen(std::unique_ptr<Screen> screen);
    void pushScreen(std::unique_ptr<Screen> screen);
    void popScreen();
    void update(float delta);
    void draw();

    ~ScreenManager() {
        if (currentScreen) {
            currentScreen->unload(globalRegistry);
        }
        while (!screenStack.empty()) {
            screenStack.back()->unload(globalRegistry);
            screenStack.pop_back();
        }
    }
};