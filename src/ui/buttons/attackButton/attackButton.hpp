#include "raylib.h"
#include "raylib-cpp.hpp"
#include <entt/entt.hpp>

#include "defines/components/components.hpp"
#include "defines/components/entityComponents.hpp"
#include "utils/logs.h"

#include "buttons.hpp"

class attackButton : public buttonHandler {
public:
	attackButton(entt::entity playerEntity)  {
		this->playerEntity = playerEntity;
	}

	void buttonRelease() override {
		auto stamina = registry->try_get<endurance>(playerEntity);
		APP_LOG("Attack button released! Player stamina: %.2f", stamina ? stamina->stamina : -1.0f);
		registry->emplace_or_replace<attack_intent>(playerEntity);
	}

private:
	entt::entity playerEntity;
};