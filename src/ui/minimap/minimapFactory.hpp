#include "ui/minimap/minimap.hpp"
#include <entt/entt.hpp>
#include "defines/components/components.hpp"

class MinimapFactory {
public:
    static entt::entity create(entt::registry& registry, entt::entity playerEntity, float mapWidth, float mapHeight) {
        entt::entity minimapEntity = registry.create();
        
        registry.emplace<is_ui>(minimapEntity);
        registry.emplace<script>(minimapEntity).bind<minimap>(minimapEntity, registry, playerEntity, mapWidth, mapHeight);
        
        return minimapEntity;
    }
};
