#include "gameScreen.hpp"

#include "entt/entt.hpp"

#include "engine.hpp"
#include "entities/game/entityFactory.hpp"
#include "defines/events/gestureEvents.hpp"

// TODO: rimuovere
void GameScreen::handleTap(TapEvent& e) {
	gesture = "Tap at (" + std::to_string(e.position.x) + ", " + std::to_string(e.position.y) + ")";
}
void GameScreen::handleSwipe(SwipeEvent& e) {
	gesture = "Swipe from (" + std::to_string(e.start.x) + ", " + std::to_string(e.start.y) + 
			  ") to (" + std::to_string(e.end.x) + ", " + std::to_string(e.end.y) + ")";
}
void GameScreen::handleLongTap(LongTapEvent& e) {
	gesture = "Long Tap at (" + std::to_string(e.position.x) + ", " + std::to_string(e.position.y) + ")";
}

void GameScreen::loadAssets() {
}


void GameScreen::load(entt::registry& globalRegistry) {
	loadAssets();

	dispatcher.sink<TapEvent>().connect<&GameScreen::handleTap>(this);
	dispatcher.sink<SwipeEvent>().connect<&GameScreen::handleSwipe>(this);
	dispatcher.sink<LongTapEvent>().connect<&GameScreen::handleLongTap>(this);

	EntityFactory::createCombatEntity(registry, engine->getAssetManager(), "player");
}

void GameScreen::update(float delta) {
	basicUpdate(delta);
}

void GameScreen::draw() {
	basicDraw();
	DrawText(gesture.c_str(), (GetScreenWidth() - MeasureText(gesture.c_str(), 20)) / 2, GetScreenHeight() / 4, 20, DARKGRAY);
}

void GameScreen::unload(entt::registry& globalRegistry) {
	basicUnload();
}