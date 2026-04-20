#pragma once

#include "engine/gameDataTypes.hpp"

class LevelDatabase {
public:
    static LevelData getLevelData(int levelId) {
        LevelData level;
        
        // In futuro, qui potresti caricare i dati da un file (es. "level_01.json")
        // Per ora, li definiamo qui in modo centralizzato.
        if (levelId == 1) {
            level.mapTextureId = "default";
            level.mapWidth = GetScreenWidth() * 2.0f; // 3840.0f
            level.mapHeight = GetScreenHeight() * 2.0f; // 2160.0f
            level.isTiled = true;
            
            // Definiamo i nemici per questo livello:
            // cap 5, spawn immediato 3, poi 1 ogni 3 secondi fino al cap.
            level.enemies.push_back({"slime", 5, 3.0f, 3});
        } else {
            // Fallback per livelli non definiti
            level.mapTextureId = "default";
            level.mapWidth = 1920.0f;
            level.mapHeight = 1080.0f;
            level.isTiled = true;
        }
        
        return level;
    }
};