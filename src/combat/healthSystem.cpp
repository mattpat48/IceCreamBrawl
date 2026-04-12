#include "healthSystem.hpp"
#include "defines/components/components.hpp"
#include "defines/components/entityComponents.hpp"
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
			sta.status = DEAD; // Imposta lo stato su DEAD
            registry.emplace<death_event>(entity);

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
