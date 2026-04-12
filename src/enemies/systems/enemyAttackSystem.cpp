#include "enemyAttackSystem.hpp"
#include <raymath.h>
#include "utils/logs.h"

void EnemyAttackSystem::update(entt::registry& registry, entt::entity playerEntity, float dt) {
    return; //DEBUG
    if (!registry.valid(playerEntity)) return;

    auto* playerTransform = registry.try_get<transform>(playerEntity);
    if (!playerTransform) return;

    // Ottieni tutti i nemici che hanno la capacità di attaccare
    auto enemies = registry.view<transform, attack, is_enemy, status>();

    for (auto entity : enemies) {
        auto& t = enemies.get<transform>(entity);
        auto& atk = enemies.get<attack>(entity);
        auto& s = enemies.get<status>(entity);

        if (s.isDead()) continue;

        atk.updateCooldown(dt);

        float distance = Vector2Distance(playerTransform->position, t.position);

        if (distance <= atk.range) {
            // Aggiungiamo l'intento, il CombatSystem processerà il danno reale!
            registry.emplace_or_replace<attack_intent>(entity);
			//APP_LOG("Enemy %d is attacking! Distance to player: %.2f", entity, distance);
		}
    }
}