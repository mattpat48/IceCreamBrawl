#include "gameScreen.hpp"
#include "playerFactory.hpp"
#include "controllerFactory.hpp"
#include "screens/game/deathScreen.hpp"
#include "minimapFactory.hpp"
#include "mapFactory.hpp"
#include "ui/buttons/pauseButton/pauseButtonFactory.hpp"
#include "attackButtonFactory.hpp"
#include "engine.hpp"
#include <raymath.h>
#include "ui/minimap/minimap.hpp"
#include "utils/logs.h"

void GameScreen::load(entt::registry& globalRegistry) {
 
    // Colleghiamo l'evento di pausa alla funzione di questo schermo
    //registry.ctx().get<entt::dispatcher>().sink<PauseToggleEvent>().connect<&GameScreen::onPauseToggle>(this);
    registry.ctx().get<entt::dispatcher>().sink<PlayerDeathEvent>().connect<&GameScreen::onPlayerDeath>(this); // Ascolta la morte
    registry.ctx().get<entt::dispatcher>().sink<PlayerRespawnEvent>().connect<&GameScreen::onPlayerRespawn>(this); // Ascolta il respawn
 
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

    // Aggiungiamo il pulsante di pausa
    //auto pauseButton = PauseButtonFactory::create(registry, engine->getAssetManager(), {GetScreenWidth() - 70.0f, 70.0f});
    //APP_LOG("Pause button created with entity ID: %d", static_cast<int>(pauseButton));
    
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
    // L'input deve essere processato anche in pausa per poter catturare l'evento di un-pause
    updateInput();

    if (!paused) {
        // Eseguiamo la logica di gioco solo se non siamo in pausa
        updateGameLogic(delta);
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

    // Se il gioco è in pausa, disegna l'overlay di pausa sopra a tutto il resto
    //if (paused) {
    //    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
    //    const char* text = "PAUSA";
    //    int fontSize = 100;
    //    int textWidth = MeasureText(text, fontSize);
    //    DrawText(text, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - fontSize / 2, fontSize, WHITE);
    //}
}

void GameScreen::unload(entt::registry& globalRegistry) {
    // Scolleghiamo l'evento per evitare chiamate a oggetti distrutti
    registry.ctx().get<entt::dispatcher>().sink<PlayerDeathEvent>().disconnect<&GameScreen::onPlayerDeath>(this);
    registry.ctx().get<entt::dispatcher>().sink<PlayerRespawnEvent>().disconnect<&GameScreen::onPlayerRespawn>(this);
    //registry.ctx().get<entt::dispatcher>().sink<PauseToggleEvent>().disconnect<&GameScreen::onPauseToggle>(this);
    engine->getAssetManager().unloadAll();
}

void GameScreen::onPlayerDeath(const PlayerDeathEvent& event) {
    APP_LOG("PlayerDeathEvent received. Pushing DeathScreen.");
    // Il gioco continua a girare sotto, ma noi aggiungiamo la schermata di morte in cima
    engine->pushScreen(std::make_unique<DeathScreen>());
}

void GameScreen::onPlayerRespawn(const PlayerRespawnEvent& event) {
    APP_LOG("PlayerRespawnEvent received. Respawning player.");

    // 1. Ricrea il giocatore usando la factory
    PlayerSaveData pData = engine->getDataManager().getPlayerData();
    auto pStaticData = engine->getDataManager().getPlayerStaticData();
    entt::entity newPlayerEntity = PlayerFactory::create(registry, engine->getAssetManager(), pData, pStaticData);

    // 2. Aggiorna tutti i sistemi che avevano una reference alla vecchia entità
    rebindPlayer(newPlayerEntity);
}

void GameScreen::rebindPlayer(entt::entity newPlayer) {
    this->playerEntity = newPlayer;

    // Aggiorna i sistemi che dipendono dal player
    enemyMovementSystem.updatePlayerEntity(newPlayer);
    enemyAttackSystem.updatePlayerEntity(newPlayer);

    // Aggiorna gli script della UI che dipendono dal player
    auto joystickView = registry.view<script, is_joystick>();
    for (auto entity : joystickView) {
        auto* controllerScript = dynamic_cast<touchController*>(registry.get<script>(entity).instance.get());
        if (controllerScript) controllerScript->playerEntity = newPlayer;
    }

    auto minimapView = registry.view<script, is_minimap>();
    for (auto entity : minimapView) {
        auto* minimapScript = dynamic_cast<minimap*>(registry.get<script>(entity).instance.get());
        if (minimapScript) minimapScript->setPlayerEntity(newPlayer);
    }

    // Aggiungi qui altri sistemi/UI che dipendono dal player, come i pulsanti di attacco
    APP_LOG("Player rebound to all systems with new entity ID: %d", static_cast<int>(newPlayer));
}