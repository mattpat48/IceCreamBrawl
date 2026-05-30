#include "gameScreen.hpp"

#include "entt/entt.hpp"
#include <cmath>

#include "engine/engine.hpp"
#include "entities/game/entityFactory.hpp"
#include "defines/events/gestureEvents.hpp"
#include "entities/game/combatEntities/player/playerScripts.hpp"

#include "utils/various.h"
#include "utils/log.h"

// TODO: rimuovere
void GameScreen::handleTap(TapEvent& e) {
	gesture = "Tap at (" + std::to_string(e.position.x) + ", " + std::to_string(e.position.y) + ")";
	ICB_LOGI("Received TapEvent at position (%d, %d) with duration %f seconds", e.position.x, e.position.y, e.duration);
}
void GameScreen::handleSwipe(SwipeEvent& e) {
	gesture = "Swipe from (" + std::to_string(e.start.x) + ", " + std::to_string(e.start.y) + 
			  ") to (" + std::to_string(e.end.x) + ", " + std::to_string(e.end.y) + ")";
	ICB_LOGI("Received SwipeEvent from (%d, %d) to (%d, %d) with duration %f seconds", e.start.x, e.start.y, e.end.x, e.end.y, e.duration);

	Directions direction = getSwipeDirection(e.start, e.end);
	auto playerPos = registry.try_get<gridInfo>(playerEntity);
	if (playerPos) {
		ICB_LOGI("Player at (%d, %d) swiped in direction %d", int(playerPos->row), int(playerPos->column), static_cast<int>(direction));
		PlayerMovementEvent moveEvent;
		moveEvent.playerEntity = playerEntity;
		moveEvent.direction = direction;
		moveEvent.originRow = playerPos->row;
		moveEvent.originColumn = playerPos->column;
		dispatcher.trigger(moveEvent);
	}
}
void GameScreen::handleLongTap(LongTapEvent& e) {
	gesture = "Long Tap at (" + std::to_string(e.position.x) + ", " + std::to_string(e.position.y) + ")";
	ICB_LOGI("Received LongTapEvent at position (%d, %d) with duration %f seconds", e.position.x, e.position.y, e.duration);
}

void GameScreen::loadAssets() {
}


void GameScreen::load(entt::registry& globalRegistry) {
	loadAssets();

	dispatcher.sink<TapEvent>().connect<&GameScreen::handleTap>(this);
	dispatcher.sink<SwipeEvent>().connect<&GameScreen::handleSwipe>(this);
	dispatcher.sink<LongTapEvent>().connect<&GameScreen::handleLongTap>(this);

	gridInstance.load(registry, dispatcher, this->engine->getMovementManager());

	playerEntity = EntityFactory::createCombatEntity(registry, engine->getAssetManager(), "player");
	registry.emplace<script>(playerEntity).bind<playerScripts>(playerEntity, registry);
	gridInstance.addToCell(0, 1, playerEntity);
	
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