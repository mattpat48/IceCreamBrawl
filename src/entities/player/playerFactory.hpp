#pragma once

#include <entt/entt.hpp>
#include "raylib.h"
#include "raylib-cpp.hpp"
#include "engine/assetManager.hpp"
#include "defines/components/components.hpp"
#include "defines/defines_player.h"
#include "entities/player/playerScripts.hpp"
#include <memory>
#include <unordered_map>
#include <string>

class PlayerFactory {
public:
    static entt::entity create(entt::registry& registry, AssetManager& assetManager, Vector2 position, Vector2 scale) {
        entt::entity playerEntity = registry.create();

        std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> playerTextures;
        
        // Usiamo l'AssetManager invece di ricaricare i file ogni volta
        playerTextures["idle"] = assetManager.loadTexture(std::string(PLAYER_SPRITESHEET_PATH) + "idle.png");
        playerTextures["attack"] = assetManager.loadTexture(std::string(PLAYER_SPRITESHEET_PATH) + "attack.png");
        playerTextures["run"] = assetManager.loadTexture(std::string(PLAYER_SPRITESHEET_PATH) + "run.png");

        registry.emplace<sprite>(playerEntity, std::move(playerTextures), "idle", PLAYER_SPRITES_H_DIMENSION, PLAYER_SPRITES_V_DIMENSION);

        registry.emplace<transform>(playerEntity, position, scale, 0.0f);
        registry.emplace<velocity>(playerEntity, 0.0f, 0.0f);
        registry.emplace<animation>(playerEntity, 0, 0, 7, 1, 0.1f, 0.0f, true, 0);
        registry.emplace<script>(playerEntity).bind<playerScripts>(playerEntity, registry);
        registry.emplace<hitFlash>(playerEntity);
        registry.emplace<status>(playerEntity, IDLE);
        
        registry.emplace<health>(playerEntity, PlayerBaseStats::MAX_HEALTH, PlayerBaseStats::MAX_HEALTH, PlayerBaseStats::REGEN_HEALTH);
        registry.emplace<endurance>(playerEntity, PlayerBaseStats::MAX_ENDURANCE, PlayerBaseStats::MAX_ENDURANCE, PlayerBaseStats::REGEN_ENDURANCE);
        registry.emplace<damage>(playerEntity, PlayerBaseStats::BASE_DAMAGE, PlayerBaseStats::BASE_DAMAGE, 0.0f, 1.0f);
        registry.emplace<attack>(playerEntity, PlayerBaseStats::ATTACK_COST, PlayerBaseStats::ATTACK_COOLDOWN, PlayerBaseStats::ATTACK_RANGE, 0.0f, 0.0f, 0.0f, 0.0f);

        return playerEntity;
    }
};