#pragma once

#include "components/components.hpp"
#include "scripts/script.hpp"
#include <entt/entt.hpp>
#include <iostream>

class buttonHandler : public Script {
public:
    buttonHandler(entt::registry& registry) {
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
                                                       (float)s->width * t->scale.x,
                                                       (float)s->height * t->scale.y})) {
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
};