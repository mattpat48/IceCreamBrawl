#include "deathScreen.hpp"
#include "engine.hpp"
#include <cmath> // Per ceil()
#include "utils/logs.h"

// Implementiamo il nuovo costruttore
DeathScreen::DeathScreen(entt::registry& gameRegistry) : notificationRegistry(gameRegistry) {}

void DeathScreen::load(entt::registry& globalRegistry) {
    // Per semplicità, usiamo il font di default. In un progetto più grande,
    // useremmo l'AssetManager.
    font = GetFontDefault();
}

void DeathScreen::update(float delta) {
    respawnTimer -= delta;

    if (respawnTimer <= 0.0f) {
        // Lancia l'evento di respawn che verrà catturato dal GameScreen
        APP_LOG("Respawn timer finished. Triggering PlayerRespawnEvent and popping DeathScreen.");
        // Usiamo il registry della GameScreen per lanciare l'evento!
        notificationRegistry.ctx().get<entt::dispatcher>().trigger<PlayerRespawnEvent>();
        engine->popScreen();
    }
}

void DeathScreen::draw() {
    // Disegna un overlay rosso semitrasparente
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(MAROON, 0.7f));

    const char* deathText = "SEI MORTO";
    int deathFontSize = 100;
    int deathTextWidth = MeasureText(deathText, deathFontSize);
    DrawText(deathText, GetScreenWidth() / 2 - deathTextWidth / 2, GetScreenHeight() / 2 - 100, deathFontSize, WHITE);

    // Disegna il countdown
    std::string countdownText = "Respawn in " + std::to_string((int)ceil(respawnTimer));
    int countdownFontSize = 40;
    int countdownTextWidth = MeasureText(countdownText.c_str(), countdownFontSize);
    DrawText(countdownText.c_str(), GetScreenWidth() / 2 - countdownTextWidth / 2, GetScreenHeight() / 2 + 20, countdownFontSize, LIGHTGRAY);
}

void DeathScreen::unload(entt::registry& globalRegistry) {
    // Nessuna risorsa specifica da scaricare per ora
}