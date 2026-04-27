#include "gameScreen.hpp"
#include "ui/controller/controllerFactory.hpp"
#include "screens/game/deathScreen.hpp"
#include "ui/minimap/minimapFactory.hpp"
#include "map/mapFactory.hpp"
#include "ui/buttons/pauseButton/pauseButtonFactory.hpp"
#include "ui/buttons/attackButton/attackButtonFactory.hpp"
#include "ui/buttons/abilityButton/abilityButtonFactory.hpp"
#include "engine/engine.hpp"
#include "player/playerFactory.hpp"
#include <entt/entt.hpp>
#include <raymath.h>
#include "utils/logs.h"

void GameScreen::load(entt::registry& globalRegistry) {
 
    // Colleghiamo l'evento di pausa alla funzione di questo schermo
    //registry.ctx().get<entt::dispatcher>().sink<PauseToggleEvent>().connect<&GameScreen::onPauseToggle>(this);
    registry.ctx().get<entt::dispatcher>().sink<PlayerDeathEvent>().connect<&GameScreen::onPlayerDeath>(this); // Ascolta la morte
    registry.ctx().get<entt::dispatcher>().sink<PlayerRespawnEvent>().connect<&GameScreen::onPlayerRespawn>(this); // Ascolta il respawn
    registry.ctx().emplace<Camera2D>(camera);
 
    // 1. Carica i dati dal manager
    PlayerSaveData pData = engine->getDataManager().getPlayerData();
    LevelData lData = engine->getDataManager().getLevelData(1);

    mapWidth = lData.mapWidth;
    mapHeight = lData.mapHeight;
    gameplayRuntime = std::make_unique<GameplayRuntime>(engine->getAssetManager(), engine->getDataManager());

    auto pStaticData = engine->getDataManager().getPlayerStaticData();
    playerEntity = PlayerFactory::create(registry, engine->getAssetManager(), pData, pStaticData);
    APP_LOG("Player created with entity ID: %d", static_cast<int>(playerEntity));

    auto btnData = engine->getDataManager().getPrimaryButtonData();
    Vector2 primaryPosition = {GetScreenWidth() * btnData.relPosX, GetScreenHeight() * btnData.relPosY};
    auto primaryAttackButton = AttackButtonFactory::create(registry, engine->getAssetManager(), btnData, primaryPosition);
    APP_LOG("Primary attack button created with entity ID: %d", static_cast<int>(primaryAttackButton));

    auto debuffBtnData = engine->getDataManager().getDebuffAbilityButtonData();
    Vector2 debuffPosition = {GetScreenWidth() * debuffBtnData.relPosX, GetScreenHeight() * debuffBtnData.relPosY};
    auto debuffAbilityButton = AbilityButtonFactory::create(registry, engine->getAssetManager(), debuffBtnData, debuffPosition, 0);
    APP_LOG("Debuff ability button created with entity ID: %d", static_cast<int>(debuffAbilityButton));

    auto buffBtnData = engine->getDataManager().getBuffAbilityButtonData();
    Vector2 buffPosition = {GetScreenWidth() * buffBtnData.relPosX, GetScreenHeight() * buffBtnData.relPosY};
    auto buffAbilityButton = AbilityButtonFactory::create(registry, engine->getAssetManager(), buffBtnData, buffPosition, 1);
    APP_LOG("Buff ability button created with entity ID: %d", static_cast<int>(buffAbilityButton));

    auto damageBtnData = engine->getDataManager().getDamageAbilityButtonData();
    Vector2 damagePosition = {GetScreenWidth() * damageBtnData.relPosX, GetScreenHeight() * damageBtnData.relPosY};
    auto damageAbilityButton = AbilityButtonFactory::create(registry, engine->getAssetManager(), damageBtnData, damagePosition, 2);
    APP_LOG("Damaging ability button created with entity ID: %d", static_cast<int>(damageAbilityButton));

    // Add map background entity
    auto mapEntity = MapFactory::create(registry, engine->getAssetManager(), lData);
    APP_LOG("Map loaded with entity ID: %d", static_cast<int>(mapEntity));

    // Add touch controller entity
    auto joyData = engine->getDataManager().getJoystickData();
    Vector2 joystickPosition = {GetScreenWidth() * joyData.relPosX, GetScreenHeight() * joyData.relPosY};
    auto joystickEntity = ControllerFactory::createTouchJoystick(registry, engine->getAssetManager(), joystickPosition, joyData.radius);
    APP_LOG("Touch joystick created with entity ID: %d", static_cast<int>(joystickEntity));
    
    // Add minimap entity
    auto minimapEntity = MinimapFactory::create(registry, playerEntity, mapWidth, mapHeight);
    APP_LOG("Minimap created with entity ID: %d", static_cast<int>(minimapEntity));

    // Aggiungiamo il pulsante di pausa
    //auto pauseButton = PauseButtonFactory::create(registry, engine->getAssetManager(), {GetScreenWidth() - 70.0f, 70.0f});
    //APP_LOG("Pause button created with entity ID: %d", static_cast<int>(pauseButton));
    
    // Inizializza orchestration gameplay con i dati livello correnti.
    gameplayRuntime->initialize(lData);
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
    // L'input deve essere processato anche in pausa per poter catturare l'evento di un-pause
    updateInput();
    if (auto* ctxCamera = registry.ctx().find<Camera2D>()) {
        *ctxCamera = camera;
    }

    if (!paused) {
        // Eseguiamo la logica di gioco solo se non siamo in pausa
        updateGameLogic(delta);
        updateCamera();
        if (auto* ctxCamera = registry.ctx().find<Camera2D>()) {
            *ctxCamera = camera;
        }

        if (gameplayRuntime) {
            gameplayRuntime->update(registry, delta, playerEntity, mapWidth, mapHeight);
        }
    }
}

void GameScreen::draw() {
    basicDraw(); // Questo disegna la mappa, gli sprite, ecc.
    
    // Disegniamo gli elementi di combattimento in 2D (con la prospettiva della telecamera)
    BeginMode2D(camera);
    if (gameplayRuntime) {
        gameplayRuntime->getCombatSystem().draw(registry);
    }
    EndMode2D();
}

void GameScreen::unload(entt::registry& globalRegistry) {
    // Scolleghiamo l'evento per evitare chiamate a oggetti distrutti
    registry.ctx().get<entt::dispatcher>().sink<PlayerDeathEvent>().disconnect<&GameScreen::onPlayerDeath>(this);
    registry.ctx().get<entt::dispatcher>().sink<PlayerRespawnEvent>().disconnect<&GameScreen::onPlayerRespawn>(this);
    //registry.ctx().get<entt::dispatcher>().sink<PauseToggleEvent>().disconnect<&GameScreen::onPauseToggle>(this);
    gameplayRuntime.reset();
    engine->getAssetManager().unloadAll();
}

void GameScreen::onPlayerDeath(const PlayerDeathEvent& event) {
    if (event.handled) return; // Se l'evento è già stato gestito, ignoralo
    event.handled = true; // Segnala che l'evento è stato gestito
    APP_LOG("PlayerDeathEvent received. Pushing DeathScreen.");
    engine->pushScreen(std::make_unique<DeathScreen>(registry));
    if (gameplayRuntime) {
        gameplayRuntime->handlePlayerDeath(registry);
    }
}

void GameScreen::onPlayerRespawn(const PlayerRespawnEvent& event) {
    if (event.handled) return; // Se l'evento è già stato gestito, ignoralo
    event.handled = true; // Segnala che l'evento è stato gestito
    APP_LOG("PlayerRespawnEvent received. Respawning player.");

    if (gameplayRuntime) {
        playerEntity = gameplayRuntime->respawnPlayer(registry, playerEntity);
        event.playerEntity = playerEntity;
    }
}