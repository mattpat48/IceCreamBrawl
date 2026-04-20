#include "enemyMovementSystem.hpp"

#include "defines/components/components.hpp"
#include "defines/components/entityComponents.hpp"
#include "defines/events.hpp"

#include <raymath.h>

static void setStatusWithEvent(entt::registry& registry, entt::entity entity, StatusType next, StatusChangeSource source) {
	auto* sta = registry.try_get<status>(entity);
	if (!sta) {
		return;
	}

	if (sta->status == next) {
		return;
	}

	const StatusType prev = sta->status;
	sta->status = next;
	registry.ctx().get<entt::dispatcher>().trigger(EntityStatusChangedEvent{entity, prev, next, source});
}

void EnemyMovementSystem::update(entt::registry& registry, entt::entity playerEntity, float dt) {
    if (!registry.valid(playerEntity)) return;

    auto* playerTransform = registry.try_get<transform>(playerEntity);
    auto* playerStatus = registry.try_get<status>(playerEntity);
    
    // Se il player non esiste o è morto, i nemici si fermano
    if (!playerTransform || (playerStatus && playerStatus->status == StatusType::DEAD)) return;

    // Trova tutti i nemici che possono muoversi (Nota: aggiungeremo "is_enemy" in components.hpp)
    auto enemies = registry.view<transform, velocity, is_enemy, status, is_aggroed, attack>();

    for (auto entity : enemies) {
        auto& t = enemies.get<transform>(entity);
        auto& v = enemies.get<velocity>(entity);
        auto& s = enemies.get<status>(entity);
		auto& aggro = enemies.get<is_aggroed>(entity);
		auto& atk = enemies.get<attack>(entity);

        if (s.status == StatusType::DEAD || s.isAttacking()) {
            v.dx = 0.0f;
            v.dy = 0.0f;
            continue;
        }

		if (aggro.aggroed) {

			float distance = Vector2Distance(playerTransform->position, t.position);

			// Se il player è nel raggio d'azione (es. 500) ma non troppo vicino (es. 100)
			if (distance < 500.0f && distance > atk.range) {
				Vector2 dir = Vector2Subtract(playerTransform->position, t.position);
				Vector2 norm = Vector2Normalize(dir);
				float speed = 150.0f; // Potrai spostare questo in un componente 'stats' in futuro
				
				v.dx = norm.x * speed * dt;
				v.dy = norm.y * speed * dt;
				setStatusWithEvent(registry, entity, StatusType::RUN, StatusChangeSource::EnemyMovement);
			} else {
				v.dx = 0.0f;
				v.dy = 0.0f;
				if (!s.isAttacking()) {
					setStatusWithEvent(registry, entity, StatusType::IDLE, StatusChangeSource::EnemyMovement);
				}
				if (distance > 500.0f) {
					aggro.aggroed = false;
					registry.ctx().get<entt::dispatcher>().trigger(EnemyAggroChangedEvent{entity, false});
				}
			}
		} else {
			// Se non è aggroed, controlla se il player è abbastanza vicino da aggroarlo
			float distance = Vector2Distance(playerTransform->position, t.position);
			if (distance < 300.0f) {
				aggro.aggroed = true;
				registry.ctx().get<entt::dispatcher>().trigger(EnemyAggroChangedEvent{entity, true});
				// Quando diventa aggro, resetta la velocità per evitare che continui il movimento casuale
				v.dx = 0.0f;
				v.dy = 0.0f;
			} else {
				// --- LOGICA DI MOVIMENTO CASUALE (WANDERING) ---
				auto& wander = registry.get_or_emplace<random_movement>(entity);
				wander.timer += dt;

				if (wander.state == WanderState::STILL) {
					// Il nemico è fermo.
					v.dx = 0.0f;
					v.dy = 0.0f;
					if (s.status == StatusType::RUN) setStatusWithEvent(registry, entity, StatusType::IDLE, StatusChangeSource::EnemyMovement);

					if (wander.timer >= wander.currentIdleDuration) {
						// Il tempo di attesa è finito. Inizia a muoversi.
						wander.state = WanderState::MOVING;
						wander.timer = 0.0f;
						setStatusWithEvent(registry, entity, StatusType::RUN, StatusChangeSource::EnemyMovement);

						// Scegli una direzione casuale
						Vector2 randomDir = { (float)GetRandomValue(-100, 100), (float)GetRandomValue(-100, 100) };
						if (Vector2LengthSqr(randomDir) > 0) {
							wander.moveDirection = Vector2Normalize(randomDir);
						} else {
							wander.moveDirection = { 1.0f, 0.0f }; // Fallback
						}
					}
				} else if (wander.state == WanderState::MOVING) {
					// Il nemico si sta muovendo.
					if (wander.timer >= wander.moveDuration) {
						// Il tempo di movimento è finito. Torna fermo.
						wander.state = WanderState::STILL;
						wander.timer = 0.0f;
						setStatusWithEvent(registry, entity, StatusType::IDLE, StatusChangeSource::EnemyMovement);
						v.dx = 0.0f;
						v.dy = 0.0f;

						// Scegli una nuova durata casuale per la prossima attesa
						wander.currentIdleDuration = (float)GetRandomValue(wander.minIdleTime * 100, wander.maxIdleTime * 100) / 100.0f;
					} else {
						// Continua a muoversi: applica la velocità per questo frame
						v.dx = wander.moveDirection.x * wander.moveSpeed * dt;
						v.dy = wander.moveDirection.y * wander.moveSpeed * dt;
					}
				}
			}
		}
	}
}