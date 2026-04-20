#pragma once

#include <entt/entt.hpp>
#include "raylib.h"

#include "defines/components/entityComponents.hpp"

// -----------------------------
// UI / screen events
// -----------------------------
struct PauseToggleEvent { mutable bool handled = false; };

// -----------------------------
// Entity lifecycle events
// -----------------------------
struct PlayerDeathEvent { mutable bool handled = false; };
struct PlayerRespawnEvent {
	mutable bool handled = false;
	mutable entt::entity playerEntity = entt::null;
};
struct EntityDiedEvent {
	entt::entity entity = entt::null;
	bool isPlayer = false;
	mutable bool handled = false;
};

// -----------------------------
// Input intent events
// -----------------------------
struct PlayerMoveIntentEvent {
	Vector2 movement = {0.0f, 0.0f};
	mutable bool handled = false;
};
struct PlayerAttackIntentEvent { mutable bool handled = false; };

// -----------------------------
// Combat / gameplay domain events
// -----------------------------
struct DamageAppliedEvent {
	entt::entity attacker = entt::null;
	entt::entity victim = entt::null;
	float amount = 0.0f;
	mutable bool handled = false;
};

enum class StatusChangeSource {
	Combat,
	Health,
	EnemyMovement,
	EntityScript
};

struct EntityStatusChangedEvent {
	entt::entity entity = entt::null;
	StatusType from = StatusType::IDLE;
	StatusType to = StatusType::IDLE;
	StatusChangeSource source = StatusChangeSource::EntityScript;
	mutable bool handled = false;
};

struct EnemyAggroChangedEvent {
	entt::entity enemy = entt::null;
	bool aggroed = false;
	mutable bool handled = false;
};