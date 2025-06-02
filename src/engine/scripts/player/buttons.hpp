#pragma once

#include "components/components.hpp"
#include "scripts/script.hpp"
#include <entt/entt.hpp>

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
                std::cout << "Button pressed!" << std::endl;
                auto ps = registry->try_get<sprite>(playerEntity);
                auto pa = registry->try_get<animation>(playerEntity);
                if (ps->currentTexture != "attack") {
                    ps->currentTexture = "attack"; // Change player texture to attack
                    pa->currentFrame = 0;
                }
            }
            a->currentFrame = 2; // Change to normal frame
        }
    }

    void onDraw() override {
        // Drawing logic for buttons if needed
    }

protected:
    entt::entity playerEntity; // Reference to the player entity
};