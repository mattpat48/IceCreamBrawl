#include "engine/playerRebindingService.hpp"

#include "defines/components/components.hpp"
#include "ui/controller/controller.hpp"
#include "ui/minimap/minimap.hpp"
#include "ui/buttons/attackButton/attackButton.hpp"
#include "utils/logs.h"

void PlayerRebindingService::rebindPlayer(entt::registry& registry, entt::entity newPlayer) {
    auto joystickView = registry.view<script, is_joystick>();
    for (auto entity : joystickView) {
        auto* controllerScript = static_cast<touchController*>(registry.get<script>(entity).instance.get());
        controllerScript->setPlayerEntity(newPlayer);
    }

    auto minimapView = registry.view<script, is_minimap>();
    for (auto entity : minimapView) {
        auto* minimapScript = static_cast<minimap*>(registry.get<script>(entity).instance.get());
        minimapScript->setPlayerEntity(newPlayer);
    }

    auto attackButtonView = registry.view<script, is_primary_attack>();
    for (auto entity : attackButtonView) {
        auto* attackButtonScript = static_cast<attackButton*>(registry.get<script>(entity).instance.get());
        attackButtonScript->setPlayerEntity(newPlayer);
    }

    APP_LOG("Player rebound to all systems with new entity ID: %d", static_cast<int>(newPlayer));
}
