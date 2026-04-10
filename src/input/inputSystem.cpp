#include "inputSystem.hpp"

InputSystem::InputSystem(entt::dispatcher& disp) : dispatcher(disp) {
    isPointerActive = false;
    lastPosition = {0.0f, 0.0f};
}

void InputSystem::update() {
    // 1. Gestione Gesti Avanzati Raylib (Swipe, Tap, Pinch ecc.)
    int gesture = GetGestureDetected();
    if (gesture != GESTURE_NONE) {
        dispatcher.trigger(GestureEvent{gesture, GetTouchPosition(0)});
    }

    // 2. Gestione base del tocco (o click del mouse su PC)
    // Raylib è comodo: GetTouchPosition(0) funge da mouse se non c'è touch
    bool currentActive = IsMouseButtonDown(MOUSE_LEFT_BUTTON) || GetTouchPointCount() > 0;
    Vector2 currentPos = GetTouchPosition(0);

    if (currentActive) {
        if (!isPointerActive) {
            // TOUCH DOWN: Il dito ha appena toccato lo schermo
            isPointerActive = true;
            lastPosition = currentPos;
            dispatcher.trigger(TouchDownEvent{currentPos, 0});
        } else {
            // TOUCH MOVE: Il dito si sta muovendo
            Vector2 delta = Vector2Subtract(currentPos, lastPosition);
            if (delta.x != 0.0f || delta.y != 0.0f) {
                dispatcher.trigger(TouchMoveEvent{currentPos, delta, 0});
                lastPosition = currentPos;
            }
        }
    } else {
        if (isPointerActive) {
            // TOUCH UP: Il dito è stato sollevato
            isPointerActive = false;
            dispatcher.trigger(TouchUpEvent{currentPos, 0});
        }
    }
}