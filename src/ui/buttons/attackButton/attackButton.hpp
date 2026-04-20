#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"
#include <entt/entt.hpp>

#include "defines/components/components.hpp"
#include "defines/components/entityComponents.hpp"
#include "defines/events.hpp"
#include "utils/logs.h"

#include "ui/buttons/buttons.hpp"

class attackButton : public buttonHandler {
public:
	attackButton() = default;

	void buttonRelease() override {
		registry->ctx().get<entt::dispatcher>().trigger(PlayerAttackIntentEvent{});
		APP_LOG("Attack button released and intent dispatched.");
	}
};