#include "menuScreen.hpp"
#include "engine/engine.hpp"
#include "screens/game/gameScreen.hpp"

void MenuScreen::load(entt::registry& globalRegistry) {
    // center the button
    playButton.x = GetScreenWidth() / 2.0f - playButton.width / 2.0f;
    playButton.y = GetScreenHeight() / 2.0f - playButton.height / 2.0f;
}

void MenuScreen::update(float delta) {
    // Simple mouse click to start game
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (mouse.x >= playButton.x && mouse.x <= playButton.x + playButton.width &&
            mouse.y >= playButton.y && mouse.y <= playButton.y + playButton.height) {
            // push GameScreen
            getEngine()->pushScreen(std::make_unique<GameScreen>());
        }
    }
}

void MenuScreen::draw() {
    DrawText("Main Menu", GetScreenWidth()/2 - MeasureText("Main Menu", 20)/2, GetScreenHeight()/2 - 80, 20, BLACK);
    DrawRectangleRec(playButton, GRAY);
    DrawText("Play", playButton.x + (playButton.width/2 - MeasureText("Play", 20)/2), playButton.y + 15, 20, WHITE);
}

void MenuScreen::unload(entt::registry& globalRegistry) {
    basicUnload();
}
