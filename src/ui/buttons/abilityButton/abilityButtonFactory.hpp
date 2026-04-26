#pragma once

#include <entt/entt.hpp>
#include <memory>
#include <unordered_map>

#include "defines/components/components.hpp"
#include "engine/assetManager.hpp"
#include "raylib.h"
#include "ui/buttons/abilityButton/abilityButton.hpp"

class AbilityButtonFactory {
public:
    static entt::entity create(entt::registry& registry,
                               AssetManager& assetManager,
                               const ButtonStaticData& bData,
                               Vector2 position,
                               int abilitySlot) {
        entt::entity abilityButtonEntity = registry.create();
        registry.emplace<is_ui>(abilityButtonEntity);

        std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> textures;
        textures["default"] = assetManager.loadTexture(bData.texturePath);
        registry.emplace<sprite>(abilityButtonEntity,
                                 std::move(textures),
                                 "default",
                                 bData.spriteWidth,
                                 bData.spriteHeight);
        registry.emplace<transform>(abilityButtonEntity, position, bData.scale, 0.0f);
        registry.emplace<animation>(abilityButtonEntity,
                                    bData.animStartFrame,
                                    bData.animStartFrame,
                                    bData.animEndFrame,
                                    bData.animRow,
                                    bData.animFrameTime,
                                    0.0f,
                                    false,
                                    Directions::DOWN);
        registry.emplace<script>(abilityButtonEntity).bind<abilityButton>(abilityButtonEntity, registry, abilitySlot);

        return abilityButtonEntity;
    }
};
