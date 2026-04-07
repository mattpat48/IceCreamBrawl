#pragma once

#include "defines/components/components.hpp"
#include "engine/scripts/script.hpp"
#include <entt/entt.hpp>
#include <iostream>

class entityMovement : public Script {
public:

    void onUpdate(float dt) override {
		auto t = getComponent<transform>();

		if (t->position.x < 0) t->position.x = 0;
		if (t->position.y < 0) t->position.y = 0;
		if (t->position.x > GetScreenWidth()) t->position.x = GetScreenWidth();
		if (t->position.y > GetScreenHeight()) t->position.y = GetScreenHeight();
    }

    void onCreate() override { }

    void onDraw() override { }
};