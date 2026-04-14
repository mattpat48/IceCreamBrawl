#include "pauseButton.hpp"
#include "utils/logs.h"
#include "defines/components/components.hpp"

void PauseButton::onCreate() {
    auto& dispatcher = registry->ctx().get<entt::dispatcher>();
    dispatcher.sink<TouchDownEvent>().connect<&PauseButton::onTouchDown>(this);

    // Calcola i bordi del pulsante per il controllo della collisione del tocco
    auto t = getComponent<transform>();
    auto s = getComponent<sprite>();
    if (t && s) {
        bounds = {
            t->position.x - (s->width * t->scale.x / 2.0f),
            t->position.y - (s->height * t->scale.y / 2.0f),
            s->width * t->scale.x,
            s->height * t->scale.y
        };
    } else {
        // Fallback se i componenti non sono presenti
        bounds = {0, 0, 0, 0};
    }
}

void PauseButton::onDestroy() {
    auto& dispatcher = registry->ctx().get<entt::dispatcher>();
    dispatcher.sink<TouchDownEvent>().disconnect<&PauseButton::onTouchDown>(this);
}

void PauseButton::onTouchDown(const TouchDownEvent& event) {
    // Il gioco può essere in pausa, ma il pulsante deve comunque funzionare.
    if (event.handled) return;

    if (CheckCollisionPointRec(event.position, bounds)) {
        // Innesca un evento globale di pausa/ripresa
        registry->ctx().get<entt::dispatcher>().trigger<PauseToggleEvent>();
        APP_LOG("Pause button clicked, PauseToggleEvent triggered.");
        event.handled = true;
    }
}