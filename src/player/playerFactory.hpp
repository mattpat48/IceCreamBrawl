#pragma once

#include <entt/entt.hpp>
#include "raylib.h"
#include "raylib-cpp.hpp"
#include "engine/assetManager.hpp"
#include "defines/components/components.hpp"
#include "engine/gameDataManager.hpp"
#include "player/playerScripts.hpp"
#include <memory>
#include <unordered_map>
#include <string>

class PlayerFactory {
public:
    static entt::entity create(entt::registry& registry, AssetManager& assetManager, const PlayerSaveData& pData, const EntityStaticData& sData) {
        entt::entity playerEntity = registry.create();
        registry.emplace<is_player>(playerEntity); // Aggiungiamo il tag is_player

        std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> playerTextures;
        
        // Usiamo l'AssetManager invece di ricaricare i file ogni volta
        playerTextures["idle"] = assetManager.loadTexture(sData.basePath + "idle.png");
        playerTextures["attack"] = assetManager.loadTexture(sData.basePath + "attack.png");
        playerTextures["run"] = assetManager.loadTexture(sData.basePath + "run.png");
        playerTextures["dead"] = assetManager.loadTexture(sData.basePath + "dead.png");
        auto projectileTexture = assetManager.loadTexture(sData.basePath + "projectile.png");

        registry.emplace<sprite>(playerEntity, std::move(playerTextures), "idle", sData.spriteWidth, sData.spriteHeight);

        registry.emplace<transform>(playerEntity, pData.spawnPosition, sData.scale, 0.0f);
        registry.emplace<velocity>(playerEntity, 0.0f, 0.0f);
        registry.emplace<animation>(playerEntity, sData.animStartFrame, sData.animStartFrame, sData.animEndFrame, sData.animRow, sData.animFrameTime, 0.0f, true, Directions::DOWN);
        registry.emplace<script>(playerEntity).bind<playerScripts>(playerEntity, registry);
        registry.emplace<collider>(playerEntity, sData.colOffsetX, sData.colOffsetY, sData.colWidth, sData.colHeight);
        registry.emplace<hit_flash>(playerEntity);
        registry.emplace<status>(playerEntity, StatusType::IDLE);
        
        // I dati di base (maxHealth, baseDamage) vengono dal database statico (sData)
        // I dati correnti (currentHealth) vengono dal salvataggio (pData)
        registry.emplace<health>(playerEntity, sData.maxHealth, pData.currentHealth, sData.healthRegenRate);
        registry.emplace<endurance>(playerEntity, sData.maxEndurance, pData.currentEndurance, sData.enduranceRegenRate);
        registry.emplace<damage>(playerEntity, sData.baseDamage, sData.baseDamage, 0.0f, 1.0f);
        // Passiamo i nuovi parametri di forma e angolo dell'attacco
        registry.emplace<attack>(playerEntity, sData.attackCost, sData.attackCooldown, sData.attackRange, sData.attackRangeType, sData.attackType, sData.attackShape, sData.attackAngle, 0.0f, 0.0f, 0.0f, 0.0f);
        registry.emplace<ranged_projectile_emitter>(playerEntity, projectileTexture, 820.0f, 24.0f, 2.5f);
        auto& loadout = registry.emplace<ability_loadout>(playerEntity);
        loadout.abilities.push_back({"Cripple", AbilityType::DEBUFFING, 12.0f, 6.0f, 0.0f, 260.0f, "cripple_attack"});
        loadout.abilities.push_back({"Fortify", AbilityType::BUFFING, 10.0f, 8.0f, 0.0f, 260.0f, "fortify_health"});
        registry.emplace<ability_targeting_state>(playerEntity);

        return playerEntity;
    }
};