#pragma once

#include "defines/components/components.hpp"

#include <iostream>

class enemyAttack : public Script {
public:

    void onCreate() override { }

    void onUpdate(float dt) override { }

    void onDraw() override { }

    virtual ~enemyAttack() = default;
};