#pragma once
#include <string>
#include <vector>
#include "raylib.h"

// 1. Strutture dati "Pure", indipendenti da EnTT e Raylib
struct PlayerSaveData {
    int level;
    float currentHealth;
    float maxHealth;
    float currentEndurance;
    float maxEndurance;
    float baseDamage;
    Vector2 spawnPosition;
    // Qui potrai aggiungere inventario, abilità sbloccate, ecc.
};

struct EnemySpawnData {
    std::string enemyType; // es. "goblin", "orc", "slime"
    int count;             // Quanti ne spawnano
    float spawnInterval;   // Se 0.0f, spawnano tutti all'inizio, altrimenti ogni X secondi
    float baseHealth;
    float baseDamage;
};

struct LevelData {
    std::string mapTextureId;
    float mapWidth;
    float mapHeight;
    bool isTiled;
    std::vector<EnemySpawnData> enemies;
};

// 2. Il Gestore dei Dati
class GameDataManager {
public:
    GameDataManager() = default;
    ~GameDataManager() = default;

    // Metodi pubblici esposti al GameScreen e alle Factory
    PlayerSaveData getPlayerData() const;
    LevelData getLevelData(int levelId) const;
};