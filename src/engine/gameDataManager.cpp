#include "gameDataManager.hpp"

PlayerSaveData GameDataManager::getPlayerData() const {
    PlayerSaveData data;
    EntityStaticData sData = EntityDatabase::getPlayerData();
    
    // SIMULAZIONE: in futuro qui leggerai un file "save_01.json" o chiamerai le API del server
    data.level = 1;
    // Su un nuovo gioco, la vita corrente è quella massima definita nel database
    data.currentHealth = sData.maxHealth;
    data.currentEndurance = sData.maxEndurance;
	data.spawnPosition = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f}; // Posizione di spawn centrale
    
    return data;
}

LevelData GameDataManager::getLevelData(int levelId) const {
    LevelData level;
    
    // SIMULAZIONE: potresti caricare "level_01.json", "level_02.json" in base a levelId
    if (levelId == 1) {
        level.mapTextureId = "default";
        level.mapWidth = GetScreenWidth();
        level.mapHeight = GetScreenHeight();
        level.isTiled = true;
        
        // Definiamo ondate o nemici statici per questo livello
        // Ora basta definire solo il TIPO, la QUANTITA' e l'INTERVALLO. Il resto è nel database.
        level.enemies.push_back({"slime", 1, 0.0f}); 
        //level.enemies.push_back({"goblin", 3, 10.0f, 50.0f, 10.0f}); // 3 goblin, 1 ogni 10 secondi
    } else {
        // Fallback o altri livelli
        level.mapWidth = 1000.0f;
        level.mapHeight = 1000.0f;
    }
    
    return level;
}
