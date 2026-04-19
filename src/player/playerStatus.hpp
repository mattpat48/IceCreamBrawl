#pragma once

#include "entities/entityStatus.hpp"

class playerStatus : public entityStatus {
public:
	playerStatus();

	void onUpdate(float deltaTime) override;
	void onDraw() override;
	void handleDeath() override;
};