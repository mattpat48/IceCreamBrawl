#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "screen.hpp"
#include <entt/entt.hpp>

class MenuScreen : public Screen {
public:
    void load(entt::registry& globalRegistry) override;
    void update(float delta) override;
    void draw() override;
    void unload(entt::registry& globalRegistry) override;
    virtual ~MenuScreen() = default;
protected:
    Rectangle playButton{0,0,200,60};
};
