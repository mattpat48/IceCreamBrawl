#include "enemyStatus.hpp"

enemyStatus::enemyStatus() : entityStatus() {
}

void enemyStatus::onUpdate(float deltaTime) {
	entityStatus::onUpdate(deltaTime);
}

void enemyStatus::handleDeath() {
	if (!registry->all_of<is_hidden>(entity)) {	
		registry->destroy(entity);
	}
}