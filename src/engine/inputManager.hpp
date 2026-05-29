#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"
#include "entt/entt.hpp"

#include "utils/log.h"
#include "defines/events/gestureEvents.hpp"

#include <cmath>

// Minimal InputManager: holds pointer to the active screen's dispatcher,
// polls simple mouse/touch input and emits Tap/LongTap/Swipe events.

class InputManager {
public:
	InputManager() = default;

	void setDispatcher(entt::dispatcher* d) { dispatcher = d; }
	void clearDispatcher() { dispatcher = nullptr; }

	// Call every frame (pass delta time, though we use absolute times below)
	void update(float dt) {
		//pollMouse();
		pollTouch();
	}

private:
	entt::dispatcher* dispatcher = nullptr;

	struct PointerState {
		bool active = false;
		Vector2 startPos = {0,0};
		Vector2 lastPos = {0,0};
		double startTime = 0.0;
		int id = -1;
	} mouseState, touchState;

	const float longTapThreshold = 0.5f; // seconds
	const float swipeMinDistance = 20.0f; // pixels

	void emitTap(const TapEvent& e) {
		if (dispatcher) dispatcher->trigger(e);
	}
	void emitLongTap(const LongTapEvent& e) {
		if (dispatcher) dispatcher->trigger(e);
	}
	void emitSwipe(const SwipeEvent& e) {
		if (dispatcher) dispatcher->trigger(e);
	}

	void pollMouse() {
		Vector2 pos = GetMousePosition();
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			mouseState.active = true;
			mouseState.startPos = pos;
			mouseState.lastPos = pos;
			mouseState.startTime = GetTime();
			mouseState.id = 0;
		} else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && mouseState.active) {
			mouseState.lastPos = pos;
		} else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && mouseState.active) {
			double endTime = GetTime();
			float duration = static_cast<float>(endTime - mouseState.startTime);
			Vector2 delta = { pos.x - mouseState.startPos.x, pos.y - mouseState.startPos.y };
			float dist = std::sqrt(delta.x*delta.x + delta.y*delta.y);

			if (dist >= swipeMinDistance) {
				SwipeEvent e{ mouseState.startPos, pos, delta, duration, mouseState.id };
				emitSwipe(e);
			} else if (duration >= longTapThreshold) {
				LongTapEvent e{ pos, duration, mouseState.id };
				emitLongTap(e);
			} else {
				TapEvent e{ pos, duration, mouseState.id };
				emitTap(e);
			}

			mouseState.active = false;
			mouseState.id = -1;
		}
	}

	void pollTouch() {
		int touchCount = GetTouchPointCount();
		if (touchCount <= 0) {
			// if previously active touch ended, finalize it
			if (touchState.active) {
				Vector2 pos = touchState.lastPos;
				double endTime = GetTime();
				float duration = static_cast<float>(endTime - touchState.startTime);
				Vector2 delta = { pos.x - touchState.startPos.x, pos.y - touchState.startPos.y };
				float dist = std::sqrt(delta.x*delta.x + delta.y*delta.y);

				if (dist >= swipeMinDistance) {
					SwipeEvent e{ touchState.startPos, pos, delta, duration, touchState.id };
					emitSwipe(e);
				} else if (duration >= longTapThreshold) {
					LongTapEvent e{ pos, duration, touchState.id };
					emitLongTap(e);
				} else {
					TapEvent e{ pos, duration, touchState.id };
					emitTap(e);
				}

				touchState.active = false;
				touchState.id = -1;
			}
			return;
		}

		// consider only first touch point for now
		Vector2 pos = GetTouchPosition(0);
		if (!touchState.active) {
			touchState.active = true;
			touchState.startPos = pos;
			touchState.lastPos = pos;
			touchState.startTime = GetTime();
			touchState.id = 0;
		} else {
			touchState.lastPos = pos;
		}
	}
};