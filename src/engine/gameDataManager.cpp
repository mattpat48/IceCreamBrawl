#include "gameDataManager.hpp"
#include "levelDatabase.hpp" // Includiamo il nuovo database dei livelli

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
    // La logica è stata spostata in LevelDatabase per una migliore organizzazione.
    return LevelDatabase::getLevelData(levelId);
}
