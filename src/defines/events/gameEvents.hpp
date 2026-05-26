#pragma once

#include "entt/entt.hpp"

#include "defines/general.hpp"

struct PlayerDeathEvent {
	entt::entity playerEntity;
};

struct EnemyDeathEvent {
	entt::entity enemyEntity;
};

struct PlayerMovementEvent {
	entt::entity playerEntity;
	Directions direction;
	Rows originRow;
	Columns originColumn;
};