#pragma once

enum class directions { UP, DOWN, RIGHT, LEFT };
enum class EntityStatus { IDLE = 0, ATTACK = 1, DODGE = 2, DEAD = 3, HURT = 4 };

// Player spritesheet paths
#define PLAYER_SPRITESHEET_PATH "sprites/player/"
#define PLAYER_SPRITES_H_DIMENSION 16
#define PLAYER_SPRITES_V_DIMENSION 16

// Player base stats
struct PlayerBaseStats {
	static constexpr float MAX_HEALTH = 100.0f;
	static constexpr float MAX_ENDURANCE = 100.0f;
	static constexpr float REGEN_HEALTH = 0.5f;
	static constexpr float REGEN_ENDURANCE = 1.0f;
	static constexpr float BASE_DAMAGE = 10.0f;
	static constexpr float ATTACK_RANGE = 50.0f;
	static constexpr float ATTACK_COOLDOWN = 1.0f;
};