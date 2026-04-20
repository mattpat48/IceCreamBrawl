#pragma once
#include "engine/gameDataTypes.hpp"
#include "defines/components/entityComponents.hpp"
#include "entities/entityDatabase.hpp"
#include "ui/uiDatabase.hpp"

class GameDataManager {
public:
    GameDataManager() = default;
    ~GameDataManager() = default;

    PlayerSaveData getPlayerData() const;
    LevelData getLevelData(int levelId) const;

    EntityStaticData getPlayerStaticData() const { return EntityDatabase::getPlayerData(); }
    EntityStaticData getEnemyStaticData(const std::string& type) const { return EntityDatabase::getEnemyData(type); }
    ButtonStaticData getPrimaryButtonData() const { return UIDatabase::getPrimaryAttackButtonData(); }
    JoystickStaticData getJoystickData() const { return UIDatabase::getJoystickData(); }
};