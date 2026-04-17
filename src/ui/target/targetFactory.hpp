#pragma once

#include <entt/entt.hpp>
#include "raylib.h"
#include "defines/components/components.hpp"
#include "defines/components/entityComponents.hpp"
#include "engine/assetManager.hpp"

class TargetFactory {
public:
    static entt::entity create(entt::registry& registry, AssetManager& assetManager, Vector2 position, entt::entity target) {
        auto entity = registry.create();

        // Transform
        registry.emplace<transform>(entity, position, Vector2{1.0f, 1.0f}, 0.0f);

        // Sprite - Assuming a texture named "target_mark" exists in assets
        auto texture = assetManager.loadTexture("assets/textures/ui/target_mark.png");
        sprite s;
        s.textures["default"] = texture;
        s.currentTexture = "default";
        s.width = texture->width / 6; // 6 frames in a single row
        s.height = texture->height;
        registry.emplace<sprite>(entity, s);

        // Animation: 6 frames, 0.1s per frame, looping
        registry.emplace<animation>(entity, 0, 0, 5, 0, 0.1f, 0.0f, true, Directions::DOWN);
		registry.emplace<is_hidden>(entity);
		registry.emplace<is_inactive>(entity);

		registry.emplace<referred_entity>(entity, target);

        registry.emplace<is_ui>(entity);
		registry.emplace<is_target>(entity);

        return entity;
    }
};