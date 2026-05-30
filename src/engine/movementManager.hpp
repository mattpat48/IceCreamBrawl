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
		currentVel->destX = targetCenter.x;
		currentVel->destY = targetCenter.y;

		currentVel->dynamic = true;

		DynState st;
		st.startX = currentTransform->position.x;
		st.startY = currentTransform->position.y;
		st.targetX = targetCenter.x;
		st.targetY = targetCenter.y;
		st.elapsed = 0.0f;

		float dist = std::sqrt(delta.x*delta.x + delta.y*delta.y);
		if (duration > 0.0f) {
			st.duration = duration;
		} else {
			float peakSpeed = std::sqrt(vel.x*vel.x + vel.y*vel.y); // px/sec
			if (peakSpeed > 1e-6f) {
				st.duration = std::max(0.05f, std::min(3.0f, dist / peakSpeed));
			} else {
				st.duration = std::max(0.1f, dist / 200.0f); // fallback
			}
		}

		dynamicStates[static_cast<uint32_t>(e)] = st;

		auto entityStatusComp = registry.try_get<entityStatus>(e);
		if (entityStatusComp) entityStatusComp->status = EntityStatus::MOVE;
		currentVel->dx = 0.0f;
		currentVel->dy = 0.0f;

		if (addShade) {
			addShadeToScroll(e, registry, delta);
		}

	}

	void addShadeToScroll(entt::entity e, entt::registry& reg, Vector2 dir) {
		// create or update trail component for the entity
		auto shPtr = reg.try_get<trail>(e);
		if (!shPtr) {
			reg.emplace<trail>(e);
		}
		auto &sh = reg.get<trail>(e);
		sh.enabled = true;
		sh.color = BLACK;

		// set trail offset opposite to direction using normalized vector
		constexpr float trailDistance = 30.0f;
		float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
		if (len > 1e-5f) {
			sh.offset = Vector2{
				-(dir.x / len) * trailDistance,
				-(dir.y / len) * trailDistance
			};
		} else {
			sh.offset = Vector2{0.0f, 0.0f};
		}
		sh.radiusX = trailDistance;
		
	}


	void updateMovements(float dt) {
		auto movingView = registry->view<transform, velocity, entityStatus>();
		movingView.each([&](auto entity, transform &t, velocity &v, entityStatus &s) {
			if (s.status != EntityStatus::MOVE) return;

			Vector2 delta = { v.destX - t.position.x, v.destY - t.position.y };

			ICB_LOGI("Updating movement for entity %d: position=(%.1f, %.1f), velocity=(%.3f, %.3f), destination=(%.1f, %.1f)", int(entity), t.position.x, t.position.y, v.dx, v.dy, v.destX, v.destY);

			if (v.dynamic) {
				// time-based eased interpolation
				auto it = dynamicStates.find(static_cast<uint32_t>(entity));
				if (it == dynamicStates.end()) {
					v.dx = 0.0f; v.dy = 0.0f;
					t.position = Vector2{ v.destX, v.destY };
					v.destX = 0.0f; v.destY = 0.0f; s.status = EntityStatus::IDLE;
					return;
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
					// disable trail if present
					if (registry) {
						auto sh = registry->try_get<trail>(entity);
						if (sh) sh->enabled = false;
					}
					dynamicStates.erase(it);
					return;
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

				// update trail offset/alpha if trail component exists
				if (registry) {
					auto sh = registry->try_get<trail>(entity);
					if (sh && sh->enabled) {
						float dirx = st.targetX - st.startX;
						float diry = st.targetY - st.startY;
						float len = std::sqrt(dirx*dirx + diry*diry);
						float dist = sh->radiusX > 0.0f ? sh->radiusX : 30.0f;
						Vector2 off = {0.0f, dist * 0.3f};
						if (len > 1e-5f) {
							off.x = -(dirx / len) * dist;
							off.y = -(diry / len) * dist + (std::abs(diry) > std::abs(dirx) ? dist * 0.2f : 0.0f);
						}
						sh->offset = off;
						// slightly fade trail as progress increases
						sh->alpha = 0.45f * (1.0f - 0.35f * p);
					}
				}
				return;

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
				return;
			}
		});
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