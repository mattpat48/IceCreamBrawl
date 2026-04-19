#pragma once

#include "raylib.h"
#include "defines/components/components.hpp"
#include "input/inputSystem.hpp"

class touchController : public Script {
public:
    touchController(entt::entity playerEntity,Vector2 joystickBasePosition, float joystickRadius) {
        this->playerEntity = playerEntity; // Initialize playerEntity to the provided entity
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

    void setPlayerEntity(entt::entity newPlayer) { playerEntity = newPlayer; }
    entt::entity getPlayerEntity() const { return playerEntity; }

    virtual ~touchController() = default;

protected:
    entt::entity playerEntity; // Riferimento all'entità del player
    Vector2 joystickBasePosition; // Base position of the joystick
    float joystickRadius; // Radius of the joystick base

    bool isDragging = false; // Indica se stiamo attualmente trascinando il joystick
    int activePointerId = -1; // Traccia quale tocco (dito) sta controllando il joystick
    Vector2 currentJoystickPos; // L'attuale posizione del dito per calcolare il delta
};