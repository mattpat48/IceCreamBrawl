#pragma once

#include <entt/entt.hpp>
#include "raylib.h"
#include "raylib-cpp.hpp"
#include "engine/assetManager.hpp"
#include "defines/components/components.hpp"

class MapFactory {
public:
    static entt::entity create(entt::registry& registry, AssetManager& assetManager, bool isTiled, float width, float height) {
        entt::entity mapEntity = registry.create();

        registry.emplace<is_map_background>(mapEntity, isTiled);
        registry.emplace<transform>(mapEntity, Vector2{0, 0}, Vector2{width, height}, 0.0f);

		std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> mapTextures;

		mapTextures["default"] = assetManager.loadTexture("resources/textures/baseTile.png");

        registry.emplace<sprite>(mapEntity, std::move(mapTextures), "default", 16, 16);

        return mapEntity;
    }
};