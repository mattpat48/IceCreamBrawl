#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"
#include <string>
#include <vector>
#include <unordered_map>

// Struttura per le informazioni delle texture di una entità
struct TextureInfo {
    std::string name;
    std::string path;
};

// Struttura per le informazioni di animazione
struct AnimationInfo {
    int startFrame = 0;
    int endFrame = 7;
    float frameTime = 0.1f;
    int row = 0;
	int direction = 0; // 0: up, 1: right, 2: down, 3: left
};

// Struttura per le informazioni di trasformazione
struct TransformInfo {
    float posX = 0.0f;
    float posY = 0.0f;
    float scaleX = 1.0f;
    float scaleY = 1.0f;
    float rotation = 0.0f;
};

// Struttura per le informazioni di velocità
struct VelocityInfo {
    float dx = 0.0f;
    float dy = 0.0f;
};

// Struttura principale che contiene tutte le info di una entità
struct EntityInfo {
    std::string id;
    std::vector<TextureInfo> textures;
    int spriteWidth = 32;
    int spriteHeight = 32;
    TransformInfo transform;
    VelocityInfo velocity;
    AnimationInfo animation;
    // Ulteriori info per combat possono essere aggiunte qui
};

// Database di entità predefinite
class EntityDatabase {
public:
    static const EntityDatabase& getInstance() {
        static EntityDatabase instance;
        return instance;
    }

    const EntityInfo* getCombatEntityInfo(const std::string& entityId) const {
        auto it = combatData.find(entityId);
        if (it != combatData.end()) {
            return &it->second;
        }
        return nullptr;
    }

    const EntityInfo* getEntityInfo(const std::string& entityId) const {
        auto it = entityData.find(entityId);
        if (it != entityData.end()) {
            return &it->second;
        }
        return nullptr;
    }

private:
    std::unordered_map<std::string, EntityInfo> combatData;
    std::unordered_map<std::string, EntityInfo> entityData;

    EntityDatabase() {
        // Database di entità predefinite
        entityData["player"] = EntityInfo{
            .id = "player",
            .textures = {
                {"idle", "resources/sprites/player/idle.png"},
                {"attack", "resources/sprites/player/attack.png"},
                {"death", "resources/sprites/player/death.png"}
            },
            .spriteWidth = 96,
            .spriteHeight = 80,
            .transform = {
                .posX = GetScreenWidth() / 2.0f,
                .posY = GetScreenHeight() / 2.0f,
                .scaleX = 4.0f,
                .scaleY = 4.0f,
                .rotation = 0.0f
            },
            .velocity = {.dx = 0.0f, .dy = 0.0f},
            .animation = {
                .startFrame = 0,
                .endFrame = 7,
                .frameTime = 0.1f,
                .row = 1,
                .direction = 1
            }
        };

        entityData["enemy"] = EntityInfo{
            .id = "enemy",
            .textures = {
                {"idle", "resources/sprites/enemy/idle.png"},
                {"walk", "resources/sprites/enemy/walk.png"},
                {"attack", "resources/sprites/enemy/attack.png"}
            },
            .spriteWidth = 32,
            .spriteHeight = 32,
            .transform = {
                .posX = 200.0f,
                .posY = 150.0f,
                .scaleX = 1.0f,
                .scaleY = 1.0f,
                .rotation = 0.0f
            },
            .velocity = {.dx = 0.0f, .dy = 0.0f},
            .animation = {
                .startFrame = 0,
                .endFrame = 6,
                .frameTime = 0.12f,
                .row = 0,
                .direction = 0
            }
        };
    }
};

