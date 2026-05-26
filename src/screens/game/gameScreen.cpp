#include "gameScreen.hpp"

#include "engine.hpp"
#include "scripts/entities/entityFactory.hpp"

void GameScreen::loadAssets() {
	//engine->getAssetManager().loadTexture("resources/sprites/player/idle.png");
	//engine->getAssetManager().loadTexture("resources/sprites/player/attack.png");
	//engine->getAssetManager().loadTexture("resources/sprites/player/death.png");
}


void GameScreen::load(entt::registry& globalRegistry) {
	loadAssets();
	EntityFactory::createEntity(registry, engine->getAssetManager(), "player");
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