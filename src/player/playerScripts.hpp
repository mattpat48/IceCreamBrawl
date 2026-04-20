#pragma once

#include "defines/components/components.hpp"
#include "defines/events.hpp"
#include "player/playerStatus.hpp"

class playerScripts : public Script {
public:

    void onCreate() override {
        playerStatusScript.bind<playerStatus>(entity, *registry);

        auto& dispatcher = registry->ctx().get<entt::dispatcher>();
        dispatcher.sink<PlayerMoveIntentEvent>().connect<&playerScripts::onMoveIntent>(this);
        dispatcher.sink<PlayerAttackIntentEvent>().connect<&playerScripts::onAttackIntent>(this);
    }

    void onDestroy() override {
        auto& dispatcher = registry->ctx().get<entt::dispatcher>();
        dispatcher.sink<PlayerMoveIntentEvent>().disconnect<&playerScripts::onMoveIntent>(this);
        dispatcher.sink<PlayerAttackIntentEvent>().disconnect<&playerScripts::onAttackIntent>(this);
    }

    void onUpdate(float dt) override {
        playerStatusScript.instance->onUpdate(dt);
    }

    void onDraw() override {
        playerStatusScript.instance->onDraw();
    }

    void onMoveIntent(const PlayerMoveIntentEvent& event) {
        if (event.handled) {
            return;
        }

        auto* movement = registry->try_get<velocity>(entity);
        auto* playerStatusComp = registry->try_get<status>(entity);
        if (!movement || !playerStatusComp) {
            return;
        }

        if (playerStatusComp->isAttacking()) {
            movement->dx = 0.0f;
            movement->dy = 0.0f;
            return;
        }

        movement->dx = event.movement.x;
        movement->dy = event.movement.y;

        if (Vector2LengthSqr(event.movement) > 0.0f) {
            playerStatusComp->status = StatusType::RUN;
        } else if (playerStatusComp->status == StatusType::RUN) {
            playerStatusComp->status = StatusType::IDLE;
        }

        event.handled = true;
    }

    void onAttackIntent(const PlayerAttackIntentEvent& event) {
        if (event.handled) {
            return;
        }

        registry->emplace_or_replace<attack_intent>(entity);
        event.handled = true;
    }

    virtual ~playerScripts() = default;

protected:
    script playerStatusScript;
};