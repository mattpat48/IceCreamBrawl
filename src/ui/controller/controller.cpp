#include "controller.hpp"

#include "utils/logs.h"

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
	Vector2 movement = {0.0f, 0.0f};

	if (isDragging) {
		Vector2 delta = Vector2Subtract(currentJoystickPos, joystickBasePosition);
		float distance = Vector2Length(delta);
		
		Vector2 clampedDelta = delta;
		if (distance > joystickRadius) {
			clampedDelta = Vector2Scale(Vector2Normalize(delta), joystickRadius);
		}
		if (t) t->position = Vector2Add(joystickBasePosition, clampedDelta);

		// Calcola il vettore di movimento e pubblicalo come intent.
		float length = Vector2Length(clampedDelta);
		if (length > 0.0f) {
			Vector2 normalized = Vector2Normalize(clampedDelta);
			movement = { normalized.x * length / joystickRadius * 350.0f * dt, normalized.y * length / joystickRadius * 350.0f * dt };
		}
	} else {
		reset();
	}

	registry->ctx().get<entt::dispatcher>().trigger(PlayerMoveIntentEvent{movement});
}

void touchController::reset() {
	auto t = getComponent<transform>();
	if (t) t->position = joystickBasePosition;
	currentJoystickPos = joystickBasePosition;
}
