#include "enemyMovementSystem.hpp"
#include "defines/components/components.hpp"
#include <raymath.h>

void EnemyMovementSystem::update(entt::registry& registry, entt::entity playerEntity, float dt) {
	return; // DEBUG
    if (!registry.valid(playerEntity)) return;

    auto* playerTransform = registry.try_get<transform>(playerEntity);
    auto* playerStatus = registry.try_get<status>(playerEntity);
    
    // Se il player non esiste o è morto, i nemici si fermano
    if (!playerTransform || (playerStatus && playerStatus->status == DEAD)) return;

    // Trova tutti i nemici che possono muoversi (Nota: aggiungeremo "is_enemy" in components.hpp)
    auto enemies = registry.view<transform, velocity, is_enemy, status, is_aggroed, attack>();

    for (auto entity : enemies) {
        auto& t = enemies.get<transform>(entity);
        auto& v = enemies.get<velocity>(entity);
        auto& s = enemies.get<status>(entity);
		auto& aggro = enemies.get<is_aggroed>(entity);
		auto& atk = enemies.get<attack>(entity);

        if (s.status == DEAD || s.isAttacking()) {
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
				s.status = RUN;
			} else {
				v.dx = 0.0f;
				v.dy = 0.0f;
				if (!s.isAttacking()) {
					s.status = IDLE;
				}
				if (distance > 500.0f) {
					aggro.aggroed = false;
				}
			}
		} else {
			// Se non è aggroed, controlla se il player è abbastanza vicino da aggroarlo
			float distance = Vector2Distance(playerTransform->position, t.position);
			if (distance < 300.0f) {
				aggro.aggroed = true;
			}
		}
	}
}