#include "raylib.h"
#include "raylib-cpp.hpp"
#include <entt/entt.hpp>

#include "defines/components/components.hpp"
#include "defines/components/entityComponents.hpp"

#include "buttons.hpp"

class attackButton : public buttonHandler {
public:
	attackButton(entt::entity playerEntity)  {
		this->playerEntity = playerEntity;
	}

	void buttonRelease() override {
		registry->emplace_or_replace<attackIntent>(playerEntity);
	}

private:
	entt::entity playerEntity;
};