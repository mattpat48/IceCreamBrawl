#pragma once

#include "defines/components/components.hpp"
#include "defines/components/entityComponents.hpp"
#include "defines/events.hpp"
#include "engine/script.hpp"
#include <entt/entt.hpp>
#include "raylib.h"
#include "raylib-cpp.hpp"

#include "utils/logs.h"

class entityStatus : public Script {
public:
    entityStatus() = default;

    void onCreate() override {
        APP_LOG("entityStatus creato per entità: %d", static_cast<uint32_t>(entity));
    }

    void onUpdate(float dt) override;

    void onDraw() override;
};