#pragma once

#include <entt/entt.hpp>
#include "raylib.h"
#include "raylib-cpp.hpp"
#include "engine/assetManager.hpp"
#include "defines/components/components.hpp"
#include "engine/gameDataManager.hpp"
#include "enemies/enemyScripts.hpp"
#include <memory>
#include <unordered_map>
#include <string>

class EnemyFactory {
public:
    static entt::entity create(entt::registry& registry, AssetManager& assetManager, const EnemySpawnData& eData, const EntityStaticData& sData, Vector2 position) {
        entt::entity enemyEntity = registry.create();

        std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> enemyTextures;
        
        // Costruiamo il percorso dinamico in base al tipo di nemico (es. "slime", "goblin")
        std::string basePath = sData.basePath;

        // Usiamo l'AssetManager per caricare/recuperare le texture
        // NOTA: Assicurati che i file esistano nelle relative cartelle!
        enemyTextures["idle"]   = assetManager.loadTexture(basePath + "idle.png");
        enemyTextures["run"]    = assetManager.loadTexture(basePath + "run.png");
        enemyTextures["attack"] = assetManager.loadTexture(basePath + "attack.png");
        enemyTextures["dead"]   = assetManager.loadTexture(basePath + "dead.png");

        registry.emplace<sprite>(enemyEntity, std::move(enemyTextures), "idle", sData.spriteWidth, sData.spriteHeight);

        // Scala e Posizione
        registry.emplace<transform>(enemyEntity, position, sData.scale, 0.0f);
        registry.emplace<velocity>(enemyEntity, 0.0f, 0.0f);
        
        // Animazione di default
        registry.emplace<animation>(enemyEntity, sData.animStartFrame, sData.animStartFrame, sData.animEndFrame, sData.animRow, sData.animFrameTime, 0.0f, true, DOWN);
        
        // Colleghiamo lo script specifico per i nemici (Status e Attacco)
        registry.emplace<script>(enemyEntity).bind<enemyScripts>(enemyEntity, registry);
        
        registry.emplace<collider>(enemyEntity, sData.colOffsetX, sData.colOffsetY, sData.colWidth, sData.colHeight);
        
        registry.emplace<hit_flash>(enemyEntity);
        registry.emplace<status>(enemyEntity, IDLE);
        
        // Le statistiche vitali e di danno derivano dai dati generati dal GameDataManager!
        registry.emplace<health>(enemyEntity, sData.maxHealth, sData.maxHealth, 0.0f); // 0 regen per i nemici
        registry.emplace<damage>(enemyEntity, sData.baseDamage, sData.baseDamage, 0.0f, 1.0f);
        registry.emplace<attack>(enemyEntity, sData.attackCost, sData.attackCooldown, sData.attackRange, sData.attackType, sData.attackShape, sData.attackAngle, 0.0f, 0.0f, 0.0f, 0.0f); 
		registry.emplace<endurance>(enemyEntity, 100.0f, 100.0f, 100.0f); // I nemici non usano l'endurance per ora

        // TAG FONDAMENTALE: Dice a MovementSystem e AttackSystem che questa entità è un nemico
        registry.emplace<is_enemy>(enemyEntity);
		registry.emplace<is_aggroed>(enemyEntity, false); // Inizialmente non è aggroed

        return enemyEntity;
    }
};