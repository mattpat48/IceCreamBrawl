#include "controller.hpp"

void touchController::onUpdate(float dt) {
	auto t = getComponent<transform>();
	auto pv = registry->try_get<velocity>(playerEntity);
	auto pStatus = registry->try_get<status>(playerEntity);

	// Handle touch input and update velocity
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || GetTouchPointCount() > 0) { // Imposta lo stato del player su RUN quando il joystick è attivo

		Vector2 inputPosition = {GetTouchPosition(0).x, GetTouchPosition(0).y};

		if (inputPosition.x < 0 || inputPosition.y < 0 ||
			inputPosition.x > GetScreenWidth() || inputPosition.y > GetScreenHeight()) {
			isDragging = false;
			reset();
			return;
		}

		if (!isDragging) {
			// Inizia a trascinare solo se il tocco iniziale è dentro il raggio del joystick
			if (Vector2Distance(inputPosition, joystickBasePosition) <= joystickRadius) {
				isDragging = true;
				if (pStatus && !pStatus->isAttacking()) {
					pStatus->status = RUN; // Imposta lo stato del player su RUN quando il joystick è attivo
				} else if (pStatus && pStatus->isAttacking()) {
					isDragging = false;
					reset();
					return; // Se il player è in attacco, non permettere di cambiare lo stato
				}
			} else {
				return; // Ignora i tocchi esterni (es. per attaccare)
			}
		}

		Vector2 delta = Vector2Subtract(inputPosition, joystickBasePosition);
		float distance = Vector2Length(delta);
		
		Vector2 clampedDelta = delta;
		if (distance > joystickRadius) {
			clampedDelta = Vector2Scale(Vector2Normalize(delta), joystickRadius);
		}
		t->position = Vector2Add(joystickBasePosition, clampedDelta);
	
		// Update player velocity based on joystick position
		float length = Vector2Length(clampedDelta);
		if (length > 0) {
			Vector2 normalized = Vector2Normalize(clampedDelta);
			pv->dx = normalized.x * length / joystickRadius * 350.0f * dt; // Scale by joystick radius
			pv->dy = normalized.y * length / joystickRadius * 350.0f * dt; // Scale by joystick radius
		} else {
			pv->dx = 0.0f;
			pv->dy = 0.0f;
		}
	} else {
		if (isDragging) {
			isDragging = false;
			if (pStatus && !pStatus->isAttacking()) {
				pStatus->status = IDLE; // Imposta lo stato del player su IDLE quando il joystick non è attivo
			}
		}
		reset();
	}

}

void touchController::reset() {
	auto t = getComponent<transform>();
	auto pv = registry->try_get<velocity>(playerEntity);
	t->position = joystickBasePosition;
	pv->dx = 0.0f;
	pv->dy = 0.0f;
}