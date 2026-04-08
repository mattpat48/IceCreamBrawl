#include "gameScreen.hpp"
#include "playerFactory.hpp"
#include "controllerFactory.hpp"
#include "minimapFactory.hpp"
#include "mapFactory.hpp"
#include "engine.hpp"
#include <raymath.h>
#include "engine/scripts/ui/minimap/minimap.hpp"
#include "utils/logs.h"

void GameScreen::load(entt::registry& globalRegistry) {
 
    Vector2 startPosition = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    Vector2 playerScale = {6.0f * scaleX, 6.0f * scaleY};
    playerEntity = PlayerFactory::create(registry, engine->getAssetManager(), startPosition, playerScale);
    APP_LOG("Player created with entity ID: %d", static_cast<int>(playerEntity));

    // Add map background entity
    auto mapEntity = MapFactory::create(registry, engine->getAssetManager(), mapWidth, mapHeight);
    APP_LOG("Map loaded with entity ID: %d", static_cast<int>(mapEntity));

    // Add touch controller entity
    Vector2 joystickPosition = {GetScreenWidth() / 4.0f, GetScreenHeight() / 4.0f * 3.0f};
    float joystickRadius = 200.0f;
    auto joystickEntity = ControllerFactory::createTouchJoystick(registry, engine->getAssetManager(), playerEntity, joystickPosition, joystickRadius);
    APP_LOG("Touch joystick created with entity ID: %d", static_cast<int>(joystickEntity));
    
    // Add minimap entity
    auto minimapEntity = MinimapFactory::create(registry, playerEntity, mapWidth, mapHeight);
    APP_LOG("Minimap created with entity ID: %d", static_cast<int>(minimapEntity));

}

void GameScreen::updateCamera() {
    // Assicurati che l'entità del giocatore sia valida
    if(!registry.valid(playerEntity)) return;

    auto& playerTransform = registry.get<transform>(playerEntity);
    auto t = registry.try_get<transform>(playerEntity);

    if (t->position.x < 20.0f) t->position.x = 20.0f;
    if (t->position.y < 20.0f) t->position.y = 20.0f;
    if (t->position.x > mapWidth - 20.0f) t->position.x = mapWidth - 20.0f;
    if (t->position.y > mapHeight - 20.0f) t->position.y = mapHeight - 20.0f;

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

void GameScreen::unload(entt::registry& globalRegistry) {
    engine->getAssetManager().unloadAll();
}