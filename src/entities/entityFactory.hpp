#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"
#include "entt/entt.hpp"

#include "defines/components/entityComponents.hpp"
#include "defines/components/combatComponents.hpp"
#include "defines/general.hpp"
#include "engine/assetManager.hpp"
#include "entities/entityInfos.hpp"
#include "utils/log.h"

namespace EntityFactory {

	void addEntityComponents(entt::entity entity, const std::string& entityTypeId, entt::registry& registry, AssetManager& assetManager) {
		const EntityInfo* info = EntityDatabase::getInstance().getGameEntityInfo(entityTypeId);
		if (!info) {
			info = EntityDatabase::getInstance().getGameEntityInfo("undefined");
		}

		std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> textures;
		for (const auto& texInfo : info->spriteData.texturePaths) {
			textures[texInfo.first] = assetManager.loadTexture(texInfo.second);
		}

		registry.emplace<entityStatus>(entity,
			info->eStatusData.status
		);

		registry.emplace<sprite>(entity,
			std::move(textures),
			info->spriteData.texturePaths,
			info->spriteData.currentTexture,
			info->spriteData.width,
			info->spriteData.height
		);

		registry.emplace<transform>(entity,
			Vector2{info->transformData.position.x, info->transformData.position.y},
			Vector2{info->transformData.scale.x, info->transformData.scale.y},
			info->transformData.rotation
		);

		registry.emplace<velocity>(entity,
			info->velocityData.dynamic,
			info->velocityData.dx,
			info->velocityData.dy,
			info->velocityData.destX,
			info->velocityData.destY,
			info->velocityData.maxSpeedX,
			info->velocityData.maxSpeedY
		);

		registry.emplace<animation>(entity,
			info->animationData.currentFrame,
			info->animationData.startFrame,
			info->animationData.endFrame,
			info->animationData.row,
			info->animationData.frameTime,
			info->animationData.timer,
			info->animationData.isPlaying,
			info->animationData.direction
		);
	}

	void addCombatComponents(entt::entity entity, const std::string& entityTypeId, entt::registry& registry, AssetManager& assetManager) {
		// TODO: prendere i dati dal database
	}

	void addGridComponents(entt::entity entity, const std::string& entityTypeId, entt::registry& registry, AssetManager& assetManager, Rows row, Columns column, bool isExclusive) {
		registry.emplace<gridInfo>(entity,
			row, column, isExclusive
		);
	}

	entt::entity createEntity(entt::registry& registry, AssetManager& assetManager, const std::string& entityTypeId) {
		entt::entity entity = registry.create();
		
		addEntityComponents(entity, entityTypeId, registry, assetManager);
		
		return entity;
	}

	entt::entity createCombatEntity(entt::registry& registry, AssetManager& assetManager, const std::string& entityTypeId) {
		entt::entity entity = registry.create();
		
		addEntityComponents(entity, entityTypeId, registry, assetManager);
		addCombatComponents(entity, entityTypeId, registry, assetManager);
		
		return entity;
	}

	entt::entity createGridEntity(entt::registry& registry, AssetManager& assetManager, const std::string& entityTypeId,
								  Rows row, Columns column, bool isExclusive) {
		entt::entity entity = registry.create();
		
		addEntityComponents(entity, entityTypeId, registry, assetManager);
		addCombatComponents(entity, entityTypeId, registry, assetManager);
		addGridComponents(entity, entityTypeId, registry, assetManager, row, column, isExclusive);
		
		return entity;
	}
} // namespace EntityFactory