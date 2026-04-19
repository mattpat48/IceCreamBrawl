#pragma once

#include <entt/entt.hpp>

class HealthSystem {
public:
	void update(entt::registry& registry, float dt);
	void resetEntities(entt::registry& registry);
};