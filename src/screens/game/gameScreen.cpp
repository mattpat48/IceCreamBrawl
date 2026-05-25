#include "gameScreen.hpp"

void GameScreen::load(entt::registry& globalRegistry) {
}

void GameScreen::update(float delta) {
	basicUpdate(delta);
}

void GameScreen::draw() {
	basicDraw();
}

void GameScreen::unload(entt::registry& globalRegistry) {
	basicUnload();
}