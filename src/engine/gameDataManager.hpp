#pragma once
#include <string>
#include <vector>
#include "raylib.h"
#include "defines/components/entityComponents.hpp"
#include "entities/entityDatabase.hpp"
#include "ui/uiDatabase.hpp"

struct EntityGraphicData {
    std::string textureId;
    int width;
    int height;
    int collisionWidth;
    int collisionHeight;
};

// 1. Strutture dati "Pure", indipendenti da EnTT e Raylib
struct PlayerSaveData {
    int level;
    // Solo i dati che cambiano e devono essere salvati
    float currentHealth;
    float currentEndurance;
    Vector2 spawnPosition;
};

struct EnemySpawnData {
    std::string enemyType; // es. "goblin", "orc", "slime"
    int count;             // Quanti ne spawnano
    float spawnInterval;   // Se 0.0f, spawnano tutti all'inizio, altrimenti ogni X secondi
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

    // Accesso ai database statici (Grafica, Collider, UI)
    EntityStaticData getPlayerStaticData() const { return EntityDatabase::getPlayerData(); }
    EntityStaticData getEnemyStaticData(const std::string& type) const { return EntityDatabase::getEnemyData(type); }
    ButtonStaticData getPrimaryButtonData() const { return UIDatabase::getPrimaryAttackButtonData(); }
    JoystickStaticData getJoystickData() const { return UIDatabase::getJoystickData(); }
};