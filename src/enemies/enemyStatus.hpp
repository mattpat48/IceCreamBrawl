#include "raylib.h"
#include "raylib-cpp.hpp"
#include <entt/entt.hpp>

#include "defines/components/components.hpp"
#include "defines/components/entityComponents.hpp"

#include "entities/entityStatus.hpp"

class enemyStatus : public entityStatus {
public:
    enemyStatus();
    void onUpdate(float deltaTime);
    void handleDeath() override;
};