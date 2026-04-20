#pragma once

#include <entt/entt.hpp>
#include "raylib.h"

// Evento per mettere in pausa/riprendere il gioco
struct PauseToggleEvent { mutable bool handled = false; };
struct PlayerDeathEvent { mutable bool handled = false; };
struct PlayerRespawnEvent {
	mutable bool handled = false;
	mutable entt::entity playerEntity = entt::null;
};
struct PlayerMoveIntentEvent {
	Vector2 movement = {0.0f, 0.0f};
	mutable bool handled = false;
};
struct PlayerAttackIntentEvent { mutable bool handled = false; };