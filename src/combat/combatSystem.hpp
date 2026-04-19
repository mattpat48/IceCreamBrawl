#pragma once

#include <entt/entt.hpp>

class CombatSystem {
public:
	void update(entt::registry& registry, float dt);
	void draw(entt::registry& registry);
};