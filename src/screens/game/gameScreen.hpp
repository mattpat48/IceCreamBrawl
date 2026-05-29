#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"
#include "entt/entt.hpp"

#include "screens/screen.hpp"
#include "defines/events/gestureEvents.hpp"
#include "entities/game/uiEntities/grid/grid.hpp"

class GameScreen : public Screen {
public:

	void load(entt::registry& globalRegistry) override;
	void update(float delta) override;
	void draw() override;
	void unload(entt::registry& globalRegistry) override;

	void handleTap(TapEvent& e);
	void handleSwipe(SwipeEvent& e);
	void handleLongTap(LongTapEvent& e);

	void loadAssets();
	virtual ~GameScreen() = default;
protected:
	entt::entity playerEntity;
	entt::entity enemyEntity;

	std::string gesture;

	grid gridInstance;
};