#include "screenManager.hpp"

void ScreenManager::setScreen(std::unique_ptr<Screen> screen) {
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

void ScreenManager::pushScreen(std::unique_ptr<Screen> screen) {
	if (screen) {
		screen->load(globalRegistry);
		screenStack.push_back(std::move(screen));
		currentScreen = screenStack.back().get();
	}
}

void ScreenManager::popScreen() {
	if (!screenStack.empty()) {
		screenStack.back()->unload(globalRegistry);
		screenStack.pop_back();
		currentScreen = screenStack.empty() ? nullptr : screenStack.back().get();
	}
}

void ScreenManager::update(float delta) {
	if (currentScreen) {
		currentScreen->update(delta);
	}
}

void ScreenManager::draw() {
	for (auto& screen : screenStack) {
		screen->draw();
	}
}