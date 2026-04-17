#pragma once

#include "engine/gameDataManager.hpp" // Per LevelData e EnemySpawnData
#include <vector>
#include <string>

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
            
            // Definiamo i nemici per questo livello
            level.enemies.push_back({"slime", 5, 3.0f}); // 5 slime, 1 ogni 3 secondi
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