#include "controller.hpp"

void touchController::onCreate() {
	// Ci iscriviamo agli eventi dell'InputSystem appena lo script viene creato
	auto& dispatcher = registry->ctx().get<entt::dispatcher>();
	dispatcher.sink<TouchDownEvent>().connect<&touchController::onTouchDown>(this);
	dispatcher.sink<TouchMoveEvent>().connect<&touchController::onTouchMove>(this);
	dispatcher.sink<TouchUpEvent>().connect<&touchController::onTouchUp>(this);
	
	currentJoystickPos = joystickBasePosition;
}

void touchController::onDestroy() {
	// Ci disiscriviamo per evitare crash quando lo schermo viene distrutto
	auto& dispatcher = registry->ctx().get<entt::dispatcher>();
	dispatcher.sink<TouchDownEvent>().disconnect<&touchController::onTouchDown>(this);
	dispatcher.sink<TouchMoveEvent>().disconnect<&touchController::onTouchMove>(this);
	dispatcher.sink<TouchUpEvent>().disconnect<&touchController::onTouchUp>(this);
}

void touchController::onTouchDown(const TouchDownEvent& event) {
	if (event.handled) return; // Se l'evento è già stato consumato da un bottone UI, ignoralo

	if (!isDragging && Vector2Distance(event.position, joystickBasePosition) <= joystickRadius) {
		isDragging = true;
		activePointerId = event.pointerId;
		currentJoystickPos = event.position;
		event.handled = true; // Il joystick consuma il tocco
	}
}

void touchController::onTouchMove(const TouchMoveEvent& event) {
	if (isDragging && event.pointerId == activePointerId) {
		currentJoystickPos = event.position;
		event.handled = true; // Consumiamo il movimento
	}
}

void touchController::onTouchUp(const TouchUpEvent& event) {
	if (isDragging && event.pointerId == activePointerId) {
		isDragging = false;
		activePointerId = -1;
		reset();
		event.handled = true; // Consumiamo il rilascio
	}
}

void touchController::onUpdate(float dt) {
	auto t = getComponent<transform>();
	auto pv = registry->try_get<velocity>(playerEntity);
	auto pStatus = registry->try_get<status>(playerEntity);

	if (isDragging) {
		// Se il player attacca (ad es. premendo un altro tasto nel frattempo), interrompiamo il movimento
		if (pStatus && pStatus->isAttacking()) {
			isDragging = false;
			activePointerId = -1;
			reset();
			return;
		}

		if (pStatus && !pStatus->isAttacking()) {
			pStatus->status = StatusType::RUN; // Imposta lo stato del player su StatusType::RUN
		}

		Vector2 delta = Vector2Subtract(currentJoystickPos, joystickBasePosition);
		float distance = Vector2Length(delta);
		
		Vector2 clampedDelta = delta;
		if (distance > joystickRadius) {
			clampedDelta = Vector2Scale(Vector2Normalize(delta), joystickRadius);
		}
		if (t) t->position = Vector2Add(joystickBasePosition, clampedDelta);
	
		// Calcola e applica la velocità
		float length = Vector2Length(clampedDelta);
		if (length > 0 && pv) {
			Vector2 normalized = Vector2Normalize(clampedDelta);
			pv->dx = normalized.x * length / joystickRadius * 350.0f * dt; // Scale by joystick radius
			pv->dy = normalized.y * length / joystickRadius * 350.0f * dt; // Scale by joystick radius
		} else if (pv) {
			pv->dx = 0.0f;
			pv->dy = 0.0f;
		}
	} else {
		// Se non stiamo trascinando, assicuriamoci che lo stato torni a StatusType::IDLE (se stava correndo)
		if (pStatus && pStatus->status == StatusType::RUN && !pStatus->isAttacking()) {
			pStatus->status = StatusType::IDLE;
		}
	}
}

void touchController::reset() {
	auto t = getComponent<transform>();
	auto pv = registry->try_get<velocity>(playerEntity);
	if (t) t->position = joystickBasePosition;
	if (pv) {
		pv->dx = 0.0f;
		pv->dy = 0.0f;
	}
	currentJoystickPos = joystickBasePosition;
}
