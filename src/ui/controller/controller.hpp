#pragma once

#include "raylib.h"
#include "defines/components/components.hpp"
#include "defines/events.hpp"
#include "input/inputSystem.hpp"

class touchController : public Script {
public:
    touchController(Vector2 joystickBasePosition, float joystickRadius) {
        this->joystickBasePosition = joystickBasePosition; // Store the joystick base position
        this->joystickRadius = joystickRadius; // Store the joystick radius
    }

    void onCreate() override;
    
    void onDestroy() override;

    void onUpdate(float dt) override;
    
    // Gestori degli eventi
    void onTouchDown(const TouchDownEvent& event);
    void onTouchMove(const TouchMoveEvent& event);
    void onTouchUp(const TouchUpEvent& event);

    void onDraw() override { }

    void reset();

    virtual ~touchController() = default;

protected:
    Vector2 joystickBasePosition; // Base position of the joystick
    float joystickRadius; // Radius of the joystick base

    bool isDragging = false; // Indica se stiamo attualmente trascinando il joystick
    int activePointerId = -1; // Traccia quale tocco (dito) sta controllando il joystick
    Vector2 currentJoystickPos; // L'attuale posizione del dito per calcolare il delta
};