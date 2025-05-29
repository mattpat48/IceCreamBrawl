#pragma once

#include <iostream>

#include "components/components.hpp"
#include <entt/entt.hpp>

class Screen {

protected:
    entt::registry registry;

public:

    virtual void load(entt::registry& registry) = 0;
    virtual void update(float delta) = 0;
    virtual void draw() = 0;
    virtual void unload() = 0;

    void basicUpdate(float delta) {
        
    }

    void basicDraw() {
        int count = 0;
        auto view = registry.view<transform, sprite, animation>();
        for (auto entity : view) {
            count++;

            auto& t = view.get<transform>(entity);
            auto& s = view.get<sprite>(entity);
            auto& a = view.get<animation>(entity);

            Rectangle source = {
                static_cast<float>(a.currentFrame * s.width),
                static_cast<float>(a.row * s.height),
                static_cast<float>(s.width),
                static_cast<float>(s.height)
            };

            float absScaleX = std::abs(t.scale.x);

            Rectangle dest = {
                t.position.x,
                t.position.y,
                s.width * absScaleX,
                s.height * t.scale.y
            };

            Vector2 origin = { 0.0f, 0.0f };

            if (t.scale.x < 0) {
                source.width = -source.width;
            }

            s.texture.Draw(source, dest, origin, t.rotation, WHITE);
        }
    }

    void basicUnload() {
        registry.clear();
    }

    virtual ~Screen() = default;
};
