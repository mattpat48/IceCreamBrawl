#pragma once

#include <entt/entt.hpp>

class PlayerRebindingService {
public:
    static void rebindPlayer(entt::registry& registry, entt::entity newPlayer);
};
