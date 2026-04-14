#pragma once

#include "screens/screen.hpp"
#include "defines/events.hpp"

class DeathScreen : public Screen {
public:
    DeathScreen() = default;
    virtual ~DeathScreen() = default;

    void load(entt::registry& globalRegistry) override;
    void update(float delta) override;
    void draw() override;
    void unload(entt::registry& globalRegistry) override;

private:
    float respawnTimer = 3.0f;
    raylib::Font font; // Potremmo caricarla da un AssetManager in futuro
};