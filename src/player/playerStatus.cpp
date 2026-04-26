#include "playerStatus.hpp"
#include "skills/skillsDatabase.hpp"

static void drawStatusEffectIcons(const status_effects& effects, float x, float y, float iconSize = 22.0f) {
	float offsetX = x;
	for (const auto& effect : effects.active) {
		const SkillEffectDefinition* definition = SkillsDatabase::getStatusEffectDefinition(effect.skillId);
		if (!definition) {
			continue;
		}

		Rectangle iconRect = { offsetX, y, iconSize, iconSize };
		DrawRectangleRounded(iconRect, 0.25f, 4, Fade(definition->iconColor, 0.85f));
		DrawRectangleRoundedLines(iconRect, 0.25f, 4, BLACK);

		if (!definition->shortLabel.empty()) {
			int labelSize = static_cast<int>(iconSize * 0.42f);
			int textWidth = MeasureText(definition->shortLabel.c_str(), labelSize);
			DrawText(definition->shortLabel.c_str(), static_cast<int>(offsetX + (iconSize - textWidth) / 2.0f), static_cast<int>(y + 2.0f), labelSize, WHITE);
		}

		offsetX += iconSize + 6.0f;
	}
}

playerStatus::playerStatus() : entityStatus() {
}

void playerStatus::onUpdate(float deltaTime) {
	entityStatus::onUpdate(deltaTime);
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
	auto effectsComp = getComponent<status_effects>();

	float barWidth = 250.0f;
	float barHeight = 26.0f;

	float healthPercent = healthComp->life / healthComp->maxLife;
	DrawRectangle(10, 40, barWidth * healthPercent, barHeight, RED);
	DrawRectangleLines(10, 40, barWidth, barHeight, BLACK);

	float staminaPercent = enduranceComp->stamina / enduranceComp->maxStamina;
	DrawRectangle(10, 76, barWidth * staminaPercent, barHeight, BLUE);
	DrawRectangleLines(10, 76, barWidth, barHeight, BLACK);

	if (effectsComp) {
		drawStatusEffectIcons(*effectsComp, 275.0f, 40.0f, 24.0f);
	}
}