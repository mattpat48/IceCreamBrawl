#pragma once

#include <entt/entt.hpp>

#include "defines/events.hpp"
#include "ui/buttons/buttons.hpp"
#include "utils/logs.h"

class abilityButton : public buttonHandler {
public:
    explicit abilityButton(int abilitySlot)
        : abilitySlot(abilitySlot) {
    }

    void buttonRelease() override {
        registry->ctx().get<entt::dispatcher>().trigger(PlayerAbilityIntentEvent{abilitySlot});
        APP_LOG("Ability button slot %d pressed.", abilitySlot);
    }

private:
    int abilitySlot = 0;
};
