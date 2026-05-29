#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

enum class Directions { DOWN = 0, UP = 1, LEFT = 2, RIGHT = 3 };
enum class Rows { FIRST = 0, SECOND = 1 };
enum class Columns { FIRST = 0, SECOND = 1, THIRD = 2 };
enum class EntityStatus { IDLE = 0, ATTACK = 1, DODGE = 2, DEAD = 3, HURT = 4 };

// Player base stats
struct PlayerBaseStats {
	static constexpr float MAX_HEALTH = 100.0f;
	static constexpr float MAX_ENDURANCE = 100.0f;
	static constexpr float REGEN_HEALTH = 0.5f;
	static constexpr float REGEN_ENDURANCE = 1.0f;
	static constexpr float BASE_DAMAGE = 10.0f;
	static constexpr float ATTACK_RANGE = 50.0f;
	static constexpr float ATTACK_COOLDOWN = 1.0f;
};

// In general.hpp
struct GridVariables {
    static constexpr int GRID_ROWS = 2;
    static constexpr int GRID_COLUMNS = 3;
};