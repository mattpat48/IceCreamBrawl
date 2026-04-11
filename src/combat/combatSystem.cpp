#include "combatSystem.hpp"
#include "utils/logs.h"

void CombatSystem::update(entt::registry& registry, float dt) {
    // Troviamo tutti quelli che vogliono attaccare e hanno le statistiche di attacco
    auto attackers = registry.view<attackIntent, attack, transform, endurance, status>();

    for (auto attacker : attackers) {
        auto& atk = attackers.get<attack>(attacker);
        auto& pos = attackers.get<transform>(attacker);
        auto& end = attackers.get<endurance>(attacker);
        auto& sta = attackers.get<status>(attacker);

        APP_LOG("Attacking: %d", attacker);

        if (atk.canAttack(end.stamina)) {
            sta.status = ATTACK; // Imposta lo stato su ATTACK
            end.consume(atk.cost);
            atk.currentCooldown = atk.cooldown;

            // FASE 3: Trova le vittime!
            // Facciamo una query per trovare tutti quelli che possono subire danno
            auto victims = registry.view<health, transform, status>();
            
            for (auto victim : victims) {
                // Evitiamo che l'attaccante colpisca se stesso
                if (attacker == victim) continue;

                auto& victimPos = victims.get<transform>(victim);
                
                // Calcola la distanza
                float distance = Vector2Distance(pos.position, victimPos.position);
                
                // Se la vittima è nel raggio d'azione...
                if (distance <= atk.range) {
                    // ...gli assegniamo un componente temporaneo "DannoSubito"!
                    float damageAmount = registry.get<damage>(attacker).currentDamage;
                    registry.emplace<damageReceived>(victim, damageAmount);
                }
            }
        }
    
        APP_LOG("Finished processing attack for: %d", attacker);
        // Rimuoviamo l'intento di attaccare, l'azione è stata processata
        registry.remove<attackIntent>(attacker);
        APP_LOG("Removed attackIntent for: %d", attacker);
    }

    auto cooldowners = registry.view<attack>();
    for (auto entity : cooldowners) {
        auto& atk = cooldowners.get<attack>(entity);
        atk.updateCooldown(dt);
    }
}
