#pragma once

#include <entt/entt.hpp>
#include "raylib.h"
#include "ui/buttons/attackButton/attackButton.hpp"
#include "defines/components/components.hpp"
#include "engine/assetManager.hpp"

class AttackButtonFactory {
public:
    static entt::entity create(entt::registry& registry, AssetManager& assetManager, 
							 const ButtonStaticData& bData, Vector2 position) {

		entt::entity attackButtonEntity = registry.create();
		registry.emplace<is_ui>(attackButtonEntity);
		registry.emplace<is_primary_attack>(attackButtonEntity);
        
		std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> primaryTextures;
		primaryTextures["default"] = assetManager.loadTexture(bData.texturePath);
		registry.emplace<sprite>(attackButtonEntity,
								 std::move(primaryTextures), "default",
								 bData.spriteWidth, bData.spriteHeight);
		registry.emplace<transform>(attackButtonEntity, position, bData.scale, 0.0f);
		registry.emplace<animation>(attackButtonEntity, bData.animStartFrame, bData.animStartFrame, bData.animEndFrame, bData.animRow, bData.animFrameTime, 0.0f, false, Directions::DOWN);
		registry.emplace<script>(attackButtonEntity).bind<attackButton>(attackButtonEntity, registry);

        return attackButtonEntity;
    }
};