#pragma once

#include "defines/components/components.hpp"
#include "engine/scripts/script.hpp"
#include <entt/entt.hpp>
#include <iostream>

class buttonHandler : public Script {
public:

    void onCreate() override { }

    void onUpdate(float dt) override;
    void onDraw() override { }

    virtual void buttonRelease() = 0;
};