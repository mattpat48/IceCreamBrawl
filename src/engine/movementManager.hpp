#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"
#include "entt/entt.hpp"

#include "defines/components/entityComponents.hpp"
#include "defines/general.hpp"

#include "utils/log.h"
#include <unordered_map>
#include <algorithm>

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
					  Vector2 targetCenter, Vector2 vel, bool addShade = false) {
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

	void handleDynamicScroll(entt::entity e, entt::registry& registry,
							 Vector2 targetCenter, Vector2 vel, float duration = 0.0f, bool addShade = false) {
		auto currentTransform = registry.try_get<transform>(e);
		auto currentVel = registry.try_get<velocity>(e);

		if (!(currentTransform && currentVel)) return;

		Vector2 delta = Vector2Subtract(targetCenter, currentTransform->position);
		// set destination
		currentVel->destX = targetCenter.x;
		currentVel->destY = targetCenter.y;

		// mark dynamic
		currentVel->dynamic = true;

		// initialize dynamic state: store start/target and compute duration
		DynState st;
		st.startX = currentTransform->position.x;
		st.startY = currentTransform->position.y;
		st.targetX = targetCenter.x;
		st.targetY = targetCenter.y;
		st.elapsed = 0.0f;

		// determine duration: prefer explicit duration param, otherwise infer from 'vel' (interpreted as peak px/sec)
		float dist = std::sqrt(delta.x*delta.x + delta.y*delta.y);
		if (duration > 0.0f) {
			st.duration = duration;
		} else {
			float peakSpeed = std::sqrt(vel.x*vel.x + vel.y*vel.y); // px/sec
			if (peakSpeed > 1e-6f) {
				// naive estimate: duration = distance / peakSpeed, but clamp to a reasonable range
				st.duration = std::max(0.05f, std::min(3.0f, dist / peakSpeed));
			} else {
				st.duration = std::max(0.1f, dist / 200.0f); // fallback
			}
		}

		dynamicStates[static_cast<uint32_t>(e)] = st;

		auto entityStatusComp = registry.try_get<entityStatus>(e);
		if (entityStatusComp) entityStatusComp->status = EntityStatus::MOVE;
		// zero per-frame dx/dy initially (we will set them each frame in updateMovements)
		currentVel->dx = 0.0f;
		currentVel->dy = 0.0f;

	}

	//void addShadeToScroll() {

	//}

	void updateMovements(float dt) {
		auto movingView = registry->view<transform, velocity, entityStatus>();
		for (auto entity : movingView) {
			auto& t = movingView.get<transform>(entity);
			auto& v = movingView.get<velocity>(entity);
			auto& s = movingView.get<entityStatus>(entity);
			if (s.status != EntityStatus::MOVE) continue;

			Vector2 delta = { v.destX - t.position.x, v.destY - t.position.y };

			ICB_LOGI("Updating movement for entity %d: position=(%.1f, %.1f), velocity=(%.3f, %.3f), destination=(%.1f, %.1f)", int(entity), t.position.x, t.position.y, v.dx, v.dy, v.destX, v.destY);

			if (v.dynamic) {
				// time-based eased interpolation
				auto it = dynamicStates.find(static_cast<uint32_t>(entity));
				if (it == dynamicStates.end()) {
					v.dx = 0.0f; v.dy = 0.0f;
					t.position = Vector2{ v.destX, v.destY };
					v.destX = 0.0f; v.destY = 0.0f; s.status = EntityStatus::IDLE;
					continue;
				}

				DynState &st = it->second;

				st.elapsed += dt;
				float p = st.duration > 0.0f ? (st.elapsed / st.duration) : 1.0f;
				if (p >= 1.0f) {
					// finish
					v.dx = 0.0f; v.dy = 0.0f;
					t.position = Vector2{ st.targetX, st.targetY };
					v.destX = 0.0f; v.destY = 0.0f;
					s.status = EntityStatus::IDLE;
					dynamicStates.erase(it);
					continue;
				}

				// ease in-out cubic
				float eased;
				if (p < 0.5f) eased = 4.0f * p * p * p;
				else {
					float q = (2.0f * p) - 2.0f;
					eased = 0.5f * q * q * q + 1.0f;
				}

				float wantX = st.startX + (st.targetX - st.startX) * eased;
				float wantY = st.startY + (st.targetY - st.startY) * eased;

				// per-frame displacement to reach eased position this frame
				v.dx = wantX - t.position.x;
				v.dy = wantY - t.position.y;
				continue;

			} else {
				// non-dynamic behavior: keep previous logic (per-frame dx/dy already set elsewhere)
				float absDx = std::abs(v.dx);
				float absDy = std::abs(v.dy);
				const bool reachedX = (std::abs(delta.x) > 0.0f) && (std::abs(delta.x) <= absDx);
				const bool reachedY = (std::abs(delta.y) > 0.0f) && (std::abs(delta.y) <= absDy);

				if (reachedX || reachedY) {
					ICB_LOGI("Entity %d reached destination: position=(%.1f, %.1f)", int(entity), t.position.x, t.position.y);
					v.dx = 0.0f; v.dy = 0.0f;
					t.position = Vector2{ v.destX, v.destY };
					v.destX = 0.0f; v.destY = 0.0f;
					s.status = EntityStatus::IDLE;
				}
			}
		}
	}

protected:
	entt::registry* registry = nullptr;
	entt::dispatcher* dispatcher = nullptr;

	struct DynState {
		// time-based interpolation state
		float startX = 0.0f;
		float startY = 0.0f;
		float targetX = 0.0f;
		float targetY = 0.0f;
		float elapsed = 0.0f;
		float duration = 0.5f; // seconds
	};

	std::unordered_map<uint32_t, DynState> dynamicStates;
};