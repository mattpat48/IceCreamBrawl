#pragma once

#pragma once

#include "components/components.hpp"

class Controller : public Script {
   public:
    void onCreate() override {
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