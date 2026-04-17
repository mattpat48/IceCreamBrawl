#include "playerStatus.hpp"

playerStatus::playerStatus() : entityStatus() {
}

void playerStatus::onUpdate(float deltaTime) {
	entityStatus::onUpdate(deltaTime);
	auto statusComp = getComponent<status>();
	if (statusComp->isDead()) {
		handleDeath();
	}
}

void playerStatus::handleDeath() {
	if (!registry->all_of<is_hidden>(entity)) {
		registry->ctx().get<entt::dispatcher>().trigger<PlayerDeathEvent>();
		registry->emplace<is_hidden>(entity); // Nascondi il player morto
	}
}

void playerStatus::onDraw() {
	entityStatus::onDraw();
	auto healthComp = getComponent<health>();
	auto enduranceComp = getComponent<endurance>();

	float healthPercent = healthComp->life / healthComp->maxLife;
	DrawRectangle(10, 40, 200 * healthPercent, 20, RED);
	DrawRectangleLines(10, 40, 200, 20, BLACK);

	float staminaPercent = enduranceComp->stamina / enduranceComp->maxStamina;
	DrawRectangle(10, 70, 200 * staminaPercent, 20, BLUE);
	DrawRectangleLines(10, 70, 200, 20, BLACK);
}