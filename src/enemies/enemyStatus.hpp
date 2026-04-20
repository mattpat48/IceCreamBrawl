#pragma once

#include "entities/entityStatus.hpp"

class enemyStatus : public entityStatus {
public:
    enemyStatus();
    void onUpdate(float deltaTime) override;
    void handleDeath() override;
};