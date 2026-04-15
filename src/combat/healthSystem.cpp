#include "healthSystem.hpp"
#include "defines/components/components.hpp"
#include "defines/components/entityComponents.hpp"
#include "defines/events.hpp"
#include "utils/logs.h"

void HealthSystem::update(entt::registry& registry, float dt) {
    // Trova chiunque abbia ricevuto danno e abbia una salute
    auto hurtEntities = registry.view<damage_received, health, status>();

    for (auto entity : hurtEntities) {
        auto& dmg = hurtEntities.get<damage_received>(entity);
        auto& hp = hurtEntities.get<health>(entity);
		auto& sta = hurtEntities.get<status>(entity);

        hp.consume(dmg.amount);
        APP_LOG("Entity %d received %.2f damage! Remaining HP: %.2f", entity, dmg.amount, hp.life);

        // Attiva l'hit flash se l'entità possiede il componente
        if (auto* hf = registry.try_get<hit_flash>(entity)) {
            hf->filter = RED; // Colora lo sprite di rosso
            hf->timeFlash = 0.15f; // Durata del flash: 150 millisecondi
        }

        // Rimuovi l'evento del danno
        registry.remove<damage_received>(entity);

        // È morto?
        if (hp.life <= 0) {
			sta.status = StatusType::DEAD; // Imposta lo stato su StatusType::DEAD
            APP_LOG("Entity %d is dead!", entity);
        }
    }

    auto regenEntities = registry.view<health, endurance>();
    for (auto entity : regenEntities) {
        auto& hp = regenEntities.get<health>(entity);
        auto& st = regenEntities.get<endurance>(entity);

        hp.regen(hp.regenRate * dt);
        st.regen(st.regenRate * dt);
    }

    // Aggiorna i timer degli hit flash
    auto flashEntities = registry.view<hit_flash>();
    for (auto entity : flashEntities) {
        auto& hf = flashEntities.get<hit_flash>(entity);
        if (hf.timeFlash > 0.0f) {
            hf.timeFlash -= dt;
            if (hf.timeFlash <= 0.0f) {
                hf.filter = WHITE; // Ripristina il colore originale
            }
        }
    }
}

void HealthSystem::resetEntities(entt::registry& registry) {
    auto entities = registry.view<health, status, is_aggroed>();
    for (auto entity : entities) {
        auto& hp = entities.get<health>(entity);
        auto& st = entities.get<status>(entity);
        auto& aggro = entities.get<is_aggroed>(entity);

        hp.reset();
        st.status = StatusType::IDLE; // Resetta lo stato a StatusType::IDLE
        aggro.aggroed = false; // Resetta l'aggro
    }
}