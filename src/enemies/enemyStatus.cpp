#include "enemyStatus.hpp"

enemyStatus::enemyStatus() : entityStatus() {
}

void enemyStatus::onUpdate(float deltaTime) {
	entityStatus::onUpdate(deltaTime);
}

void enemyStatus::handleDeath() {
	if (!registry->all_of<is_hidden>(entity)) {	
		registry->ctx().get<entt::dispatcher>().trigger(EntityDiedEvent{entity, false});
		registry->destroy(entity);
	}
}