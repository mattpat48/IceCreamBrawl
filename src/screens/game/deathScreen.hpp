#pragma once

#include "screens/screen.hpp"
#include "defines/events.hpp"

class DeathScreen : public Screen {
public:
    // Modifichiamo il costruttore per accettare il registry della schermata di gioco
    DeathScreen(entt::registry& gameRegistry);
    virtual ~DeathScreen() = default;

    void load(entt::registry& globalRegistry) override;
    void update(float delta) override;
    void draw() override;
    void unload(entt::registry& globalRegistry) override;

private:
    entt::registry& notificationRegistry; // Il registry a cui inviare l'evento di respawn
    float respawnTimer = 3.0f;
    raylib::Font font; // Potremmo caricarla da un AssetManager in futuro
};