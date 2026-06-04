#include "gameScreen.hpp"

#include "entt/entt.hpp"
#include <cmath>

#include "engine/engine.hpp"
#include "entities/entityFactory.hpp"
#include "defines/events/gestureEvents.hpp"
#include "entities/game/combatEntities/player/playerScripts.hpp"

#include "utils/various.h"
#include "utils/log.h"

// TODO: rimuovere
void GameScreen::handleTap(TapEvent& e) {
	basicHandleTap(e);
}
void GameScreen::handleSwipe(SwipeEvent& e) {
	basicHandleSwipe(e);

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
	basicHandleLongTap(e);
}

void GameScreen::loadAssets() {
}


void GameScreen::load(entt::registry& globalRegistry) {
	loadAssets();

	dispatcher.sink<TapEvent>().connect<&GameScreen::handleTap>(this);
	dispatcher.sink<SwipeEvent>().connect<&GameScreen::handleSwipe>(this);
	dispatcher.sink<LongTapEvent>().connect<&GameScreen::handleLongTap>(this);

	gridInstance.load(registry, dispatcher, this->engine->getMovementManager());

	playerEntity = EntityFactory::createGridEntity(registry, engine->getAssetManager(), "player", Rows::FIRST, Columns::SECOND, true);
	registry.emplace<script>(playerEntity).bind<playerScripts>(playerEntity, registry);
	gridInstance.addToCell(0, 1, playerEntity);
	
	//enemyEntity = EntityFactory::createGridEntity(registry, engine->getAssetManager(), "enemy", Rows::SECOND, Columns::SECOND, true);
	//gridInstance.addToCell(1, 1, enemyEntity);
	enemyEntity = EntityFactory::createCombatEntity(registry, engine->getAssetManager(), "enemy");
	
}

void GameScreen::update(float delta) {
	basicUpdate(delta);
}

void GameScreen::draw() {
	basicDraw();
	//DrawText(gesture.c_str(), (GetScreenWidth() - MeasureText(gesture.c_str(), 20)) / 2, GetScreenHeight() / 4, 20, DARKGRAY);
}

void GameScreen::unload(entt::registry& globalRegistry) {
	basicUnload();
}