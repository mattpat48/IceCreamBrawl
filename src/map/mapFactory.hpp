#pragma once

#include <entt/entt.hpp>
#include "raylib.h"
#include "raylib-cpp.hpp"
#include "engine/assetManager.hpp"
#include "defines/components/components.hpp"
#include "engine/gameDataManager.hpp"

class MapFactory {
public:
    static entt::entity create(entt::registry& registry, AssetManager& assetManager, const LevelData& lData) {
        entt::entity mapEntity = registry.create();

        registry.emplace<is_map_background>(mapEntity, lData.isTiled);
        registry.emplace<transform>(mapEntity, Vector2{0, 0}, Vector2{lData.mapWidth, lData.mapHeight}, 0.0f);

		std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> mapTextures;

        std::string texturePath = "resources/textures/" + lData.mapTextureId + ".png";
        if (lData.mapTextureId == "default") {
            texturePath = "resources/textures/baseTile.png"; // Fallback texture attuale
        }

		mapTextures["default"] = assetManager.loadTexture(texturePath);

        registry.emplace<sprite>(mapEntity, std::move(mapTextures), "default", 16, 16);

        return mapEntity;
    }
};