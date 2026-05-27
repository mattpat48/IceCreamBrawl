#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"
#include "entt/entt.hpp"

#include "defines/components/entityComponents.hpp"
#include "defines/components/combatComponents.hpp"
#include "defines/general.hpp"
#include "engine/assetManager.hpp"
#include "entities/game/entityInfos.hpp"
#include "utils/log.h"

namespace EntityFactory {

	void addEntityComponents(entt::entity entity, const std::string& entityTypeId, entt::registry& registry, AssetManager& assetManager) {
		const EntityInfo* info = EntityDatabase::getInstance().getEntityInfo(entityTypeId);
		if (!info) {
			info = EntityDatabase::getInstance().getEntityInfo("player");
		}

		// Carica le texture dal database
		std::string defaultTexture = "idle";
		std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> textures;
		for (const auto& texInfo : info->textures) {
			textures[texInfo.name] = assetManager.loadTexture(texInfo.path);
		}

		registry.emplace<sprite>(entity,
			std::move(textures),
			defaultTexture,
			info->spriteWidth,
			info->spriteHeight
		);

		registry.emplace<transform>(entity,
			Vector2{info->transform.posX, info->transform.posY},
			Vector2{info->transform.scaleX, info->transform.scaleY},
			info->transform.rotation
		);

		registry.emplace<velocity>(entity,
			info->velocity.dx,
			info->velocity.dy
		);

		registry.emplace<animation>(entity,
			0,
			info->animation.startFrame,
			info->animation.endFrame,
			info->animation.row,
			info->animation.frameTime,
			0.0f,
			true,
			info->animation.direction
		);
	}

	void addCombatComponents(entt::entity entity, entt::registry& registry) {
		// TODO: Implement combat components
	}

	entt::entity createCombatEntity(entt::registry& registry, AssetManager& assetManager, const std::string& entityTypeId) {
		entt::entity entity = registry.create();
		
		addEntityComponents(entity, entityTypeId, registry, assetManager);
		addCombatComponents(entity, registry);
		
		return entity;
	}

	entt::entity createEntity(entt::registry& registry, AssetManager& assetManager, const std::string& entityTypeId) {
		entt::entity entity = registry.create();
		
		addEntityComponents(entity, entityTypeId, registry, assetManager);
		
		return entity;
	}

} // namespace EntityFactory