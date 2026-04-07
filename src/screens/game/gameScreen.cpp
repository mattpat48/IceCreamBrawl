#include "gameScreen.hpp"
#include "engine/scripts/entities/player/playerFactory.hpp"
#include "engine/engine.hpp"

void GameScreen::load(entt::registry& globalRegistry) {
    // Il calcolo dello scale se necessario
    Vector2 startPosition = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    Vector2 playerScale = {3.0f * scaleX, 3.0f * scaleY};

	// Add player entity
    // Passiamo il registry locale (della classe Screen) alla factory
    playerEntity = PlayerFactory::create(registry, engine->getAssetManager(), startPosition, playerScale);
}

void GameScreen::unload(entt::registry& globalRegistry) {
    engine->getAssetManager().unloadAll();
}