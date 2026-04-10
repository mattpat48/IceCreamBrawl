#pragma once

#include <entt/entt.hpp>
#include "raylib.h"
#include "ui/buttons/attackButton/attackButton.hpp"
#include "defines/components/components.hpp"
#include "engine/assetManager.hpp"

class AttackButtonFactory {
public:
    static entt::entity create(entt::registry& registry, AssetManager& assetManager, 
                             entt::entity playerEntity, Vector2 position, Vector2 scale) {

		entt::entity attackButtonEntity = registry.create();
		registry.emplace<is_ui>(attackButtonEntity);
        
		std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> primaryTextures;
		primaryTextures["default"] = std::make_shared<raylib::Texture2D>("resources/sprites/buttons/primary.png");
		registry.emplace<sprite>(attackButtonEntity,
								 std::move(primaryTextures), "default",
								 BUTTON_SPRITES_H_DIMENSION, BUTTON_SPRITES_V_DIMENSION);
		registry.emplace<transform>(attackButtonEntity, position, scale, 0.0f);
		registry.emplace<animation>(attackButtonEntity,
									BUTTON_PRIMARY_ANIMATION_START_FRAME,
									BUTTON_PRIMARY_ANIMATION_END_FRAME,
									BUTTON_PRIMARY_ANIMATION_ROW,
									0,
									BUTTON_PRIMARY_ANIMATION_FRAME_TIME,
									0.0f,
									false,
									0);
		registry.emplace<script>(attackButtonEntity).bind<attackButton>(attackButtonEntity, registry, playerEntity);

        return attackButtonEntity;
    }
};