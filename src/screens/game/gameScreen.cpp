#include "gameScreen.hpp"
#include "playerFactory.hpp"
#include "controllerFactory.hpp"
#include "minimapFactory.hpp"
#include "mapFactory.hpp"
#include "attackButtonFactory.hpp"
#include "engine.hpp"
#include <raymath.h>
#include "ui/minimap/minimap.hpp"
#include "utils/logs.h"

void GameScreen::load(entt::registry& globalRegistry) {
 
    // 1. Carica i dati dal manager
    PlayerSaveData pData = engine->getDataManager().getPlayerData();
    LevelData lData = engine->getDataManager().getLevelData(1);

    mapWidth = lData.mapWidth;
    mapHeight = lData.mapHeight;

    auto pStaticData = engine->getDataManager().getPlayerStaticData();
    playerEntity = PlayerFactory::create(registry, engine->getAssetManager(), pData, pStaticData);
    APP_LOG("Player created with entity ID: %d", static_cast<int>(playerEntity));

    auto btnData = engine->getDataManager().getPrimaryButtonData();
    Vector2 primaryPosition = {GetScreenWidth() * btnData.relPosX, GetScreenHeight() * btnData.relPosY};
    auto primaryAttackButton = AttackButtonFactory::create(registry, engine->getAssetManager(), playerEntity, btnData, primaryPosition);
    APP_LOG("Primary attack button created with entity ID: %d", static_cast<int>(primaryAttackButton));

    // Add map background entity
    auto mapEntity = MapFactory::create(registry, engine->getAssetManager(), lData);
    APP_LOG("Map loaded with entity ID: %d", static_cast<int>(mapEntity));

    // Add touch controller entity
    auto joyData = engine->getDataManager().getJoystickData();
    Vector2 joystickPosition = {GetScreenWidth() * joyData.relPosX, GetScreenHeight() * joyData.relPosY};
    auto joystickEntity = ControllerFactory::createTouchJoystick(registry, engine->getAssetManager(), playerEntity, joystickPosition, joyData.radius);
    APP_LOG("Touch joystick created with entity ID: %d", static_cast<int>(joystickEntity));
    
    // Add minimap entity
    auto minimapEntity = MinimapFactory::create(registry, playerEntity, mapWidth, mapHeight);
    APP_LOG("Minimap created with entity ID: %d", static_cast<int>(minimapEntity));
    
    // Inizializza lo spawner prendendo i dati dal level data!
    enemySpawnSystem.init(lData.enemies);
}

void GameScreen::updateCamera() {
    // Assicurati che l'entità del giocatore sia valida
    if(!registry.valid(playerEntity)) return;

    auto& playerTransform = registry.get<transform>(playerEntity);
    auto t = registry.try_get<transform>(playerEntity);

    if (t->position.x < 40.0f) t->position.x = 40.0f;
    if (t->position.y < 40.0f) t->position.y = 40.0f;
    if (t->position.x > mapWidth - 40.0f) t->position.x = mapWidth - 40.0f;
    if (t->position.y > mapHeight - 40.0f) t->position.y = mapHeight - 40.0f;

    // 1. Logica Deadzone (La telecamera lo segue se si allontana dal centro)
    float deadzoneX = GetScreenWidth() / 4.0f;  // tolleranza asse X
    float deadzoneY = GetScreenHeight() / 4.0f; // tolleranza asse Y

    // Calcola la distanza tra il centro della telecamera e il player
    float diffX = playerTransform.position.x - camera.target.x;
    float diffY = playerTransform.position.y - camera.target.y;

    // Se supera la deadzone, muovi il target della camera
    if (diffX > deadzoneX) camera.target.x += (diffX - deadzoneX);
    else if (diffX < -deadzoneX) camera.target.x += (diffX + deadzoneX);

    if (diffY > deadzoneY) camera.target.y += (diffY - deadzoneY);
    else if (diffY < -deadzoneY) camera.target.y += (diffY + deadzoneY);

    // 2. Limiti della Mappa (Clamping)
    // Impedisce alla telecamera di mostrare oltre i bordi della mappa.
    float minX = camera.offset.x; // Non andare a sinistra dello 0
    float maxX = mapWidth - (GetScreenWidth() - camera.offset.x);
    float minY = camera.offset.y; // Non andare in alto dello 0
    float maxY = mapHeight - (GetScreenHeight() - camera.offset.y);

    camera.target.x = Clamp(camera.target.x, minX, maxX);
    camera.target.y = Clamp(camera.target.y, minY, maxY);
}

void GameScreen::update(float delta) {
    if (!paused) {
        basicUpdate(delta);
        updateCamera();

        combatManager.update(registry, delta);
        healthManager.update(registry, delta);

        enemySpawnSystem.update(registry, engine->getAssetManager(), engine->getDataManager(), delta, mapWidth, mapHeight);
        enemyMovementSystem.update(registry, playerEntity, delta);
        enemyAttackSystem.update(registry, playerEntity, delta);
    }
}

void GameScreen::draw() {
    basicDraw(); // Questo disegna la mappa, gli sprite, ecc.
    
    // Disegniamo gli elementi di combattimento in 2D (con la prospettiva della telecamera)
    BeginMode2D(camera);
    combatManager.draw(registry);
    EndMode2D();
    
    // Se hai elementi UI extra da disegnare, puoi metterli qui sotto
}

void GameScreen::unload(entt::registry& globalRegistry) {
    engine->getAssetManager().unloadAll();
}