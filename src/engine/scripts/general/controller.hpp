#pragma once

#pragma once

#include "components/components.hpp"

class wasdController : public Script {
   public:
    void onCreate() {
        // Initialization logic
    }

    void onUpdate(float dt) override {
        auto v = getComponent<velocity>();
        auto t = getComponent<transform>();

        // Handle input and update velocity
        if (IsKeyDown(KEY_W)) {
            v->dy = -200.0f * dt;
        } else if (IsKeyDown(KEY_S)) {
            v->dy = 200.0f * dt;
        } else {
            v->dy = 0.0f;
        }

        if (IsKeyDown(KEY_A)) {
            v->dx = -200.0f * dt;
        } else if (IsKeyDown(KEY_D)) {
            v->dx = 200.0f * dt;
        } else {
            v->dx = 0.0f;
        }
    }

    void onDraw() override {
        // Drawing logic if needed
    }
};

class changeDirectionController : public Script {
public:

    changeDirectionController(entt::entity player, entt::registry& registry) {
        this->entity = player; // Store the player entity for reference
        this->registry = &registry; // Store the registry reference
    }

    void onCreate() override {
    }

    void onUpdate(float dt) override {
        auto a = getComponent<animation>();

        if (IsKeyDown(KEY_W)) {
            a->row = UP; // Set animation row for moving up
        } else if (IsKeyDown(KEY_S)) {
            a->row = DOWN; // Set animation row for moving down
        }

        if (IsKeyDown(KEY_A)) {
            a->row = LEFT; // Set animation row for moving left
        } else if (IsKeyDown(KEY_D)) {
            a->row = RIGHT; // Set animation row for moving right
        }
    }

    void onDraw() override {
        // Drawing logic if needed
    }
};

class touchController : public Script {
public:
    touchController(entt::entity& player, entt::registry& registry, Vector2 joystickBasePosition, float joystickRadius) {
        this->playerEntity = player; // Store the player entity for reference
        this->joystickBasePosition = joystickBasePosition; // Store the joystick base position
        this->joystickRadius = joystickRadius; // Store the joystick radius
        this->registry = &registry; // Store the registry reference
    }

    void onCreate() override {
        // Initialization logic if needed
    }

    void onUpdate(float dt) override {
        auto s = getComponent<sprite>();
        auto v = getComponent<velocity>();
        auto t = getComponent<transform>();
        auto pv = registry->try_get<velocity>(playerEntity);

        // Handle touch input and update velocity
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || GetTouchPointCount() > 0) {

            Vector2 touchPosition = Vector2{GetMousePosition().x - s->width/2, GetMousePosition().y - s->height/2}; // Use mouse position for touch simulation
            Vector2 delta = Vector2Subtract(touchPosition, joystickBasePosition);
            float distance = Vector2Length(delta);

            if (distance > joystickRadius) {
                auto pa = registry->try_get<animation>(playerEntity);
                if (!pa->isPlaying) {
                    t->position = joystickBasePosition;
                    pv->dx = 0.0f;
                    pv->dy = 0.0f;
                    return;
                }
                delta = Vector2Scale(Vector2Normalize(delta), joystickRadius);
            }
            t->position = Vector2Add(joystickBasePosition, delta);
        
            // Update player velocity based on joystick position
            Vector2 joystickPosition = Vector2Subtract(t->position, joystickBasePosition);
            float length = Vector2Length(joystickPosition);
            if (length > 0) {
                Vector2 normalized = Vector2Normalize(joystickPosition);
                pv->dx = normalized.x * length / joystickRadius * 200.0f * dt; // Scale by joystick radius
                pv->dy = normalized.y * length / joystickRadius * 200.0f * dt; // Scale by joystick radius
            } else {
                pv->dx = 0.0f;
                pv->dy = 0.0f;
            }
        } else {
            t->position = joystickBasePosition;
            pv->dx = 0.0f;
            pv->dy = 0.0f;
        }

    }

    void onDraw() override {
        
    }

protected:
    entt::entity playerEntity; // Store the player entity for reference
    Vector2 joystickBasePosition; // Base position of the joystick
    float joystickRadius; // Radius of the joystick base
};