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
    static entt::entity create(entt::registry& registry, AssetManager& assetManager, const EnemySpawnData& eData, Vector2 position) {
        entt::entity enemyEntity = registry.create();

        std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> enemyTextures;
        
        // Costruiamo il percorso dinamico in base al tipo di nemico (es. "slime", "goblin")
        std::string basePath = "resources/sprites/enemies/" + eData.enemyType + "/";

        // Usiamo l'AssetManager per caricare/recuperare le texture
        // NOTA: Assicurati che i file esistano nelle relative cartelle!
        enemyTextures["idle"]   = assetManager.loadTexture(basePath + "idle.png");
        enemyTextures["run"]    = assetManager.loadTexture(basePath + "run.png");
        enemyTextures["attack"] = assetManager.loadTexture(basePath + "attack.png");
        enemyTextures["dead"]   = assetManager.loadTexture(basePath + "dead.png");

        // Assumiamo che gli sprite dei nemici abbiano una dimensione fissa (es. 32x32). Cambia i valori se necessario.
        registry.emplace<sprite>(enemyEntity, std::move(enemyTextures), "idle", 64, 64);

        // Scala e Posizione
        registry.emplace<transform>(enemyEntity, position, Vector2{5.0f, 5.0f}, 0.0f);
        registry.emplace<velocity>(enemyEntity, 0.0f, 0.0f);
        
        // Animazione di default
        registry.emplace<animation>(enemyEntity, 0, 0, 5, 1, 0.1f, 0.0f, true, 0);
        
        // Colleghiamo lo script specifico per i nemici (Status e Attacco)
        registry.emplace<script>(enemyEntity).bind<enemyScripts>(enemyEntity, registry);
        
        registry.emplace<hitFlash>(enemyEntity);
        registry.emplace<status>(enemyEntity, IDLE);
        
        // Le statistiche vitali e di danno derivano dai dati generati dal GameDataManager!
        registry.emplace<health>(enemyEntity, eData.baseHealth, eData.baseHealth, 0.0f); // 0 regen per i nemici
        registry.emplace<damage>(enemyEntity, eData.baseDamage, eData.baseDamage, 0.0f, 1.0f);
        registry.emplace<attack>(enemyEntity, 0.0f, 1.5f, 100.0f, 0.0f, 0.0f, 0.0f, 0.0f); // Range = 100, Cooldown = 1.5s
		registry.emplace<endurance>(enemyEntity, 100.0f, 100.0f, 100.0f); // I nemici non usano l'endurance per ora

        // TAG FONDAMENTALE: Dice a MovementSystem e AttackSystem che questa entità è un nemico
        registry.emplace<is_enemy>(enemyEntity);
		registry.emplace<is_aggroed>(enemyEntity, false); // Inizialmente non è aggroed

        return enemyEntity;
    }
};