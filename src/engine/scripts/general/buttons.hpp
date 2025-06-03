#pragma once

#include "components/components.hpp"
#include "scripts/script.hpp"
#include <entt/entt.hpp>
#include <iostream>

class buttonHandler : public Script {
public:
    buttonHandler(entt::entity playerEntity, entt::registry& registry) : playerEntity(playerEntity) {
        this->registry = &registry;
    }

    void onCreate() override {
        // Initialization logic for button handling
    }

    void onUpdate(float dt) override {
        auto v = getComponent<velocity>();
        auto t = getComponent<transform>();
        auto a = getComponent<animation>();
        auto s = getComponent<sprite>();

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || GetTouchPointCount() > 0) {
            Vector2 mousePosition = GetMousePosition();
            if (GetTouchPointCount() > 0) {
                mousePosition = GetTouchPosition(0);
            }

            // Check if the mouse position is within the button area
            if (CheckCollisionPointRec(mousePosition, {t->position.x - (s->width) * t->scale.x / 2.0f,
                                                       t->position.y - (s->height) * t->scale.y / 2.0f,
                                                       (float)s->width * BUTTON_PRIMARY_SCALE_X,
                                                       (float)s->height * BUTTON_PRIMARY_SCALE_Y})) {
                a->currentFrame = 1; // Change to pressed frame
            } else {
                a->currentFrame = 2; // Change to normal frame
            }
        } else {
            if (a->currentFrame == 1) {
                buttonRelease();
            }
            a->currentFrame = 2; // Change to normal frame
        }
    }

    void onDraw() override {
        // Drawing logic for buttons if needed
    }

    virtual void buttonRelease() = 0;

protected:
    entt::entity playerEntity; // Reference to the player entity
};
    

class primaryButtonScript : public buttonHandler {
public:
    primaryButtonScript(entt::entity playerEntity, entt::registry& registry)
        : buttonHandler(playerEntity, registry) {}

    void buttonRelease() override {
        auto a = registry->try_get<animation>(playerEntity);
        auto s = registry->try_get<sprite>(playerEntity);
        auto stamina = registry->try_get<staminaComponent>(playerEntity);
        auto attack = registry->try_get<attackComponent>(playerEntity);

        if (a && s && stamina && attack) {
            if (s->currentTexture == "idle") {
                stamina->stamina -= attack->cost; // Reduce stamina on attack
                s->currentTexture = "attack"; // Change to attack texture on button release
                a->currentFrame = a->startFrame; // Reset animation frame to start frame
            }
        }
    }
};