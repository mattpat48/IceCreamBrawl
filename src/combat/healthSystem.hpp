#include "raylib.h"
#include "raylib-cpp.hpp"
#include <entt/entt.hpp>

#include "defines/components/components.hpp"
#include "defines/components/entityComponents.hpp"

class HealthSystem {
public:
	void update(entt::registry& registry, float dt);
	void resetEntities(entt::registry& registry);
};