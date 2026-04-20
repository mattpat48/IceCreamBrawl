#pragma once

#include <string>
#include <vector>

#include "raylib.h"

struct EntityGraphicData {
    std::string textureId;
    int width;
    int height;
    int collisionWidth;
    int collisionHeight;
};

struct PlayerSaveData {
    int level;
    float currentHealth;
    float currentEndurance;
    Vector2 spawnPosition;
};

struct EnemySpawnData {
    std::string enemyType;
    int count;
    float spawnInterval;
    int initialSpawnCount = 1;
};

struct LevelData {
    std::string mapTextureId;
    float mapWidth;
    float mapHeight;
    bool isTiled;
    std::vector<EnemySpawnData> enemies;
};