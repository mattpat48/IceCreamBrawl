#include "inputSystem.hpp"

namespace {
std::unordered_map<int, Vector2> collectCurrentPointers() {
    std::unordered_map<int, Vector2> pointers;

    const int touchCount = GetTouchPointCount();
    if (touchCount > 0) {
        pointers.reserve(static_cast<size_t>(touchCount));
        for (int i = 0; i < touchCount; ++i) {
            pointers.emplace(i, GetTouchPosition(i));
        }
        return pointers;
    }

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        pointers.emplace(-1, GetMousePosition());
    }

    return pointers;
}

Vector2 getPrimaryPointerPosition(const std::unordered_map<int, Vector2>& pointers) {
    if (pointers.empty()) {
        return GetMousePosition();
    }

    if (const auto mouseIt = pointers.find(-1); mouseIt != pointers.end()) {
        return mouseIt->second;
    }

    return pointers.begin()->second;
}
} // namespace

InputSystem::InputSystem(entt::dispatcher& disp) : dispatcher(disp) {
}

void InputSystem::update() {
    const auto currentPointers = collectCurrentPointers();

    // 1. Gestione Gesti Avanzati Raylib (Swipe, Tap, Pinch ecc.)
    int gesture = GetGestureDetected();
    if (gesture != GESTURE_NONE) {
        dispatcher.trigger(GestureEvent{gesture, getPrimaryPointerPosition(currentPointers)});
    }

    // 2. Diff tra frame corrente e precedente per emettere DOWN/MOVE/UP per ogni pointer.
    for (const auto& [pointerId, position] : currentPointers) {
        const auto previousIt = activePointers.find(pointerId);
        if (previousIt == activePointers.end()) {
            dispatcher.trigger(TouchDownEvent{position, pointerId});
            continue;
        }

        const Vector2 delta = Vector2Subtract(position, previousIt->second);
        if (delta.x != 0.0f || delta.y != 0.0f) {
            dispatcher.trigger(TouchMoveEvent{position, delta, pointerId});
        }
    }

    for (const auto& [pointerId, previousPosition] : activePointers) {
        if (currentPointers.find(pointerId) == currentPointers.end()) {
            dispatcher.trigger(TouchUpEvent{previousPosition, pointerId});
        }
    }

    activePointers = currentPointers;
}