#include "enemyStatus.hpp"

enemyStatus::enemyStatus() : entityStatus() {
}

void enemyStatus::onUpdate(float deltaTime) {
	entityStatus::onUpdate(deltaTime);
	if (getComponent<status>()->isDead()) {
		registry->destroy(entity);
	}
}