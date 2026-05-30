#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"
#include <string>
#include <vector>
#include <unordered_map>

#include "defines/components/entityComponents.hpp"
#include "defines/components/combatComponents.hpp"
#include "defines/general.hpp"

struct EntityInfo {
    std::string id;
    entityStatus eStatusData;
    sprite spriteData;
    transform transformData;
    velocity velocityData;
    animation animationData;
};

struct CombatInfo {
    std::string id;
    gridInfo gridData;
    combatStatus cStatusData;
};

// Database di entità predefinite
class EntityDatabase {
public:
    static const EntityDatabase& getInstance() {
        static EntityDatabase instance;
        return instance;
    }

    const CombatInfo* getCombatEntityInfo(const std::string& entityId) const {
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

    Vector2 getCellCenter(Rows row, Columns column) const {
        return gridPositions[static_cast<int>(row)][static_cast<int>(column)];
    }

private:
    float cellSize;
    Vector2 gridPositions[GridVariables::GRID_ROWS][GridVariables::GRID_COLUMNS];

    std::unordered_map<std::string, CombatInfo> combatData;
    std::unordered_map<std::string, EntityInfo> entityData;

    EntityDatabase() {
        cellSize = GetScreenWidth() / static_cast<float>(GridVariables::GRID_COLUMNS) + 150.0f;
        gridPositions[0][0] = Vector2{ cellSize / 2.0f, GetScreenHeight() / 3.0f * 2.0f - cellSize / 2.0f };
        gridPositions[0][1] = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 3.0f * 2.0f - cellSize / 2.0f };
        gridPositions[0][2] = Vector2{ GetScreenWidth() - cellSize / 2.0f, GetScreenHeight() / 3.0f * 2.0f - cellSize / 2.0f };
        gridPositions[1][0] = Vector2{ cellSize / 2.0f, GetScreenHeight() / 3.0f * 2.0f };
        gridPositions[1][1] = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 3.0f * 2.0f };
        gridPositions[1][2] = Vector2{ GetScreenWidth() - cellSize / 2.0f, GetScreenHeight() / 3.0f * 2.0f };

        entityData["undefined"] = EntityInfo{
            .id = "undefined",
            .eStatusData = { EntityStatus::IDLE },
            .spriteData = {
                .textures = {},
                .currentTexture = "",
                .width = 0,
                .height = 0
            },
            .transformData = {
                .position = Vector2{0.0f, 0.0f},
                .scale = Vector2{1.0f, 1.0f},
                .rotation = 0.0f
            },
            .velocityData = {
                .dx = 0.0f,
                .dy = 0.0f
            },
            .animationData = {
                .startFrame = 0,
                .endFrame = 0,
                .row = 0,
                .frameTime = 0.1f,
                .timer = 0.0f,
                .isPlaying = false,
                .direction = 0
            }
        };


        entityData["player"] = EntityInfo{
            .id = "player",
            .eStatusData = { EntityStatus::IDLE },
            .spriteData = {
                .textures = {
                    {"idle", nullptr},
                    {"attack", nullptr},
                    {"death", nullptr}
                },
                .texturePaths = {
                    {"idle", "resources/sprites/player/idle.png"},
                    {"attack", "resources/sprites/player/attack.png"},
                    {"death", "resources/sprites/player/death.png"},
                },
                .currentTexture = "idle",
                .width = 96,
                .height = 80
            },
            .transformData = {
                .position = getCellCenter(Rows::FIRST, Columns::SECOND),
                .scale = Vector2{5.0f, 5.0f},
                .rotation = 0.0f
            },
            .velocityData = {
                .dx = 0.0f,
                .dy = 0.0f
            },
            .animationData = {
                .startFrame = 0,
                .endFrame = 7,
                .row = static_cast<int>(Directions::UP),
                .frameTime = 0.1f,
                .timer = 0.0f,
                .isPlaying = true,
                .direction = static_cast<int>(Directions::UP)
            }
        };

        entityData["enemy"] = EntityInfo{
            .id = "enemy",
            .eStatusData = { EntityStatus::IDLE },
            .spriteData = {
                .textures = {
                    {"idle", nullptr},
                    {"attack", nullptr},
                    {"death", nullptr}
                },
                .texturePaths = {
                    {"idle", "resources/sprites/enemy/idle.png"},
                    {"attack", "resources/sprites/enemy/attack.png"},
                    {"death", "resources/sprites/enemy/death.png"},
                },
                .currentTexture = "idle",
                .width = 96,
                .height = 80
            },
            .transformData = {
                .position = Vector2{GetScreenWidth() / 2.0f + 150.0f, GetScreenHeight() / 2.0f},
                .scale = Vector2{4.0f, 4.0f},
                .rotation = 0.0f
            },
            .velocityData = {
                .dx = 0.0f,
                .dy = 0.0f
            },
            .animationData = {
                .startFrame = 0,
                .endFrame = 7,
                .row = static_cast<int>(Directions::DOWN),
                .frameTime = 0.1f,
                .timer = 0.0f,
                .isPlaying = true,
                .direction = static_cast<int>(Directions::DOWN)
            }
        };

        combatData["player"] = {
            .id = "player",
            .gridData = {
                .row = Rows::FIRST,
                .column = Columns::SECOND,
                .isExclusive = false
            },
            .cStatusData = {
                .status = CombatStatus::IDLE
            }
        };
    }
};

