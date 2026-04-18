#include "raylib.h"
#include "raylib-cpp.hpp"
#include <entt/entt.hpp>

#include "defines/components/components.hpp"
#include "defines/components/entityComponents.hpp"

#include "entities/entityStatus.hpp"

class playerStatus : public entityStatus {
public:
	playerStatus();

	void onUpdate(float deltaTime) override;
	void onDraw() override;
	void handleDeath() override;
};