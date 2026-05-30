#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"
#include "entt/entt.hpp"

#include "defines/components/entityComponents.hpp"
#include "defines/general.hpp"

#include "utils/log.h"

class MovementManager {
public:
	MovementManager() = default;

	void setRegistry(entt::registry* r) { registry = r; }
	void clearRegistry() { registry = nullptr; }

	void setDispatcher(entt::dispatcher* d) { dispatcher = d; }
	void clearDispatcher() { dispatcher = nullptr; }

	void update(float dt) {
		updateMovements(dt);
	}

	void handleScroll(entt::entity e, entt::registry& registry,
					  Vector2 targetCenter, Vector2 vel) {
		auto currentTransform = registry.try_get<transform>(e);
		auto currentVel = registry.try_get<velocity>(e);

		if (currentTransform && currentVel) {
			Vector2 delta = Vector2Subtract(targetCenter, currentTransform->position);
			currentVel->dx = delta.x > 0 ? vel.x : (delta.x < 0 ? -vel.x : 0.0f);
			currentVel->dy = delta.y > 0 ? vel.y : (delta.y < 0 ? -vel.y : 0.0f);
			currentVel->destX = targetCenter.x;
			currentVel->destY = targetCenter.y;
			auto entityStatusComp = registry.try_get<entityStatus>(e);
			if (entityStatusComp) {
				entityStatusComp->status = EntityStatus::MOVE;
			}
		}
	}

	void updateMovements(float dt) {
		auto movingView = registry->view<transform, velocity, entityStatus>();
		for (auto entity : movingView) {
			auto& t = movingView.get<transform>(entity);
			auto& v = movingView.get<velocity>(entity);
			auto& s = movingView.get<entityStatus>(entity);

			if (s.status == EntityStatus::MOVE) {
				Vector2 delta = { v.destX - t.position.x, v.destY - t.position.y };

				ICB_LOGI("Updating movement for entity %d: position=(%.1f, %.1f), velocity=(%.1f, %.1f), destination=(%.1f, %.1f)", int(entity), t.position.x, t.position.y, v.dx, v.dy, v.destX, v.destY);

				const float absDeltaX = std::abs(delta.x);
				const float absDeltaY = std::abs(delta.y);
				const bool reachedX = (absDeltaX > 0.0f) && (absDeltaX <= std::abs(v.dx));
				const bool reachedY = (absDeltaY > 0.0f) && (absDeltaY <= std::abs(v.dy));

				if (reachedX || reachedY) {
					ICB_LOGI("Entity %d reached destination: position=(%.1f, %.1f)", int(entity), t.position.x, t.position.y);
					v.dx = 0.0f;
					v.dy = 0.0f;
					t.position = Vector2{ v.destX, v.destY };
					v.destX = 0.0f;
					v.destY = 0.0f;
					s.status = EntityStatus::IDLE;
				}
			}
		}
	}

protected:
	entt::registry* registry = nullptr;
	entt::dispatcher* dispatcher = nullptr;
};