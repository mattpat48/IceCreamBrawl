#include "gameScreen.hpp"

#include "engine.hpp"
#include "scripts/entities/entityFactory.hpp"

void GameScreen::loadAssets() {
}


void GameScreen::load(entt::registry& globalRegistry) {
	loadAssets();
	EntityFactory::createCombatEntity(registry, engine->getAssetManager(), "player");
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