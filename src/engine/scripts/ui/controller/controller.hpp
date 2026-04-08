#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"
#include "defines/components/components.hpp"

#include "utils/logs.h"

class touchController : public Script {
public:
    touchController(entt::entity playerEntity,Vector2 joystickBasePosition, float joystickRadius) {
        this->playerEntity = playerEntity; // Initialize playerEntity to the provided entity
        this->joystickBasePosition = joystickBasePosition; // Store the joystick base position
        this->joystickRadius = joystickRadius; // Store the joystick radius
    }

    void onCreate() override { }

    void onUpdate(float dt) override;

    void onDraw() override { }

    void reset();

    virtual ~touchController() = default;

protected:
    Vector2 joystickBasePosition; // Base position of the joystick
    float joystickRadius; // Radius of the joystick base

    entt::entity playerEntity; // Reference to the player entity
    bool isDragging = false; // Indica se stiamo attualmente trascinando il joystick
};