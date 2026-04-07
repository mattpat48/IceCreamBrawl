#include "gameScreen.hpp"
#include "playerFactory.hpp"
#include "controllerFactory.hpp"
#include "engine.hpp"

void GameScreen::load(entt::registry& globalRegistry) {
    // Il calcolo dello scale se necessario
    Vector2 startPosition = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    Vector2 playerScale = {6.0f * scaleX, 6.0f * scaleY};

	// Add player entity
    // Passiamo il registry locale (della classe Screen) alla factory
    playerEntity = PlayerFactory::create(registry, engine->getAssetManager(), startPosition, playerScale);

    // Add touch controller entity
    Vector2 joystickPosition = {GetScreenWidth() / 4.0f, GetScreenHeight() / 4.0f * 3.0f};
    float joystickRadius = 200.0f;
    ControllerFactory::createTouchJoystick(registry, engine->getAssetManager(), playerEntity, joystickPosition, joystickRadius);
    

}

void GameScreen::unload(entt::registry& globalRegistry) {
    engine->getAssetManager().unloadAll();
}