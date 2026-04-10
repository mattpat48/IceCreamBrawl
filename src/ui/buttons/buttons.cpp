#include "buttons.hpp"

void buttonHandler::onCreate() {
	auto& dispatcher = registry->ctx().get<entt::dispatcher>();
	dispatcher.sink<TouchDownEvent>().connect<&buttonHandler::onTouchDown>(this);
	dispatcher.sink<TouchMoveEvent>().connect<&buttonHandler::onTouchMove>(this);
	dispatcher.sink<TouchUpEvent>().connect<&buttonHandler::onTouchUp>(this);
}

void buttonHandler::onDestroy() {
	auto& dispatcher = registry->ctx().get<entt::dispatcher>();
	dispatcher.sink<TouchDownEvent>().disconnect<&buttonHandler::onTouchDown>(this);
	dispatcher.sink<TouchMoveEvent>().disconnect<&buttonHandler::onTouchMove>(this);
	dispatcher.sink<TouchUpEvent>().disconnect<&buttonHandler::onTouchUp>(this);
}

bool buttonHandler::isPointInside(Vector2 point) {
	auto t = getComponent<transform>();
	auto s = getComponent<sprite>();
	if (!t || !s) return false;

	Rectangle bounds = {
		t->position.x - (s->width) * t->scale.x / 2.0f,
		t->position.y - (s->height) * t->scale.y / 2.0f,
		(float)s->width * t->scale.x,
		(float)s->height * t->scale.y
	};
	return CheckCollisionPointRec(point, bounds);
}

void buttonHandler::onTouchDown(const TouchDownEvent& event) {
	if (event.handled) return; // Ignora se l'evento è già stato consumato

	if (!isTracking && isPointInside(event.position)) {
		isTracking = true;
		activePointerId = event.pointerId;
		event.handled = true; // CONSUMA L'EVENTO!
		
		auto a = getComponent<animation>();
		if (a) a->currentFrame = 1; // Change to pressed frame
	}
}

void buttonHandler::onTouchMove(const TouchMoveEvent& event) {
	if (isTracking && event.pointerId == activePointerId) {
		event.handled = true; // Consuma anche i movimenti mentre stiamo premendo
		auto a = getComponent<animation>();
		if (a) {
			if (isPointInside(event.position)) {
				a->currentFrame = 1; // Pressed
			} else {
				a->currentFrame = 2; // Normal (dito trascinato fuori dal bottone)
			}
		}
	}
}

void buttonHandler::onTouchUp(const TouchUpEvent& event) {
	if (isTracking && event.pointerId == activePointerId) {
		isTracking = false;
		activePointerId = -1;
		event.handled = true; // Consuma il rilascio

		auto a = getComponent<animation>();
		if (a) a->currentFrame = 2; // Normal

		// Clicca solo se il dito è stato sollevato MENTRE era ancora sopra il bottone
		if (isPointInside(event.position)) {
			buttonRelease();
		}
	}
}
