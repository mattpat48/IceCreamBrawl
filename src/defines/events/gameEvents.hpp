#pragma once

#include "entt/entt.hpp"

#include "defines/general.hpp"

struct PlayerDeathEvent {
	bool handled = false;
	entt::entity playerEntity;
};

struct EnemyDeathEvent {
	bool handled = false;
	entt::entity enemyEntity;
};

struct PlayerMovementEvent {
	bool handled = false;
	entt::entity playerEntity;
	Directions direction;
	Rows originRow;
	Columns originColumn;
};