#pragma once

#include <cmath>
#include "raylib.h"
#include "raylib-cpp.hpp"
#include "defines/general.hpp"

Directions getSwipeDirection(const Vector2& start, const Vector2& end) {
	float dx = end.x - start.x;
	float dy = end.y - start.y;

	if (std::abs(dx) > std::abs(dy)) {
		return (dx > 0) ? Directions::RIGHT : Directions::LEFT;
	} else {
		return (dy > 0) ? Directions::DOWN : Directions::UP;
	}
}

