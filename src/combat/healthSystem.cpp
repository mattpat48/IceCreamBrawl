#include "healthSystem.hpp"

void HealthSystem::update(entt::registry& registry, float dt) {
    // Trova chiunque abbia ricevuto danno e abbia una salute
    auto hurtEntities = registry.view<damageReceived, health, status>();

    for (auto entity : hurtEntities) {
        auto& dmg = hurtEntities.get<damageReceived>(entity);
        auto& hp = hurtEntities.get<health>(entity);
		auto& sta = hurtEntities.get<status>(entity);

        hp.consume(dmg.amount);

        // Rimuovi l'evento del danno
        registry.remove<damageReceived>(entity);

        // È morto?
        if (hp.life <= 0) {
			sta.status = DEAD; // Imposta lo stato su DEAD
            registry.emplace<deathEvent>(entity);

        }
    }
}
