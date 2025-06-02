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
    virtual void unload(entt::registry& registry) = 0;

    void basicUpdate(float delta) {
        updateScripts();
        updateAnimations(delta);

        auto view = registry.view<transform, velocity>();
        for (auto entity : view) {
            auto& t = view.get<transform>(entity);
            auto& v = view.get<velocity>(entity);

            // Update position
            t.position.x += v.dx;
            t.position.y += v.dy;
        }
    }

    void basicDraw() {
        auto view = registry.view<transform, sprite, animation>();
        for (auto entity : view) {
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
                t.position.x - s.width * t.scale.x / 2.0f,
                t.position.y - s.height * t.scale.y / 2.0f,
                s.width * absScaleX,
                s.height * t.scale.y
            };

            Vector2 origin = { 0.0f, 0.0f };

            if (t.scale.x < 0) {
                source.width = -source.width;
            }

            s.textures.at(s.currentTexture)->Draw(source, dest, origin, t.rotation, WHITE);
            
        }

        auto scriptView = registry.view<script>();
        for (auto entity : scriptView) {
            auto& s = scriptView.get<script>(entity);
            if (s.instance) {
                s.instance->onDraw();
            }
        }
    }

    void basicUnload() {
        registry.clear();
    }

    void updateScripts() {
        float dt = GetFrameTime();
        auto scriptView = registry.view<script>();

        for (auto entity : scriptView) {
            auto& s = scriptView.get<script>(entity);
            if (s.instance) {
                s.instance->entity = entity;
                s.instance->registry = &registry;
                s.instance->onUpdate(dt);
            }
        }
    }

    void updateAnimations(float dt) {
        auto view = registry.view<animation, velocity>();
        for (auto entity : view) {
            auto& a = view.get<animation>(entity);
            auto& v = view.get<velocity>(entity);

            if (std::abs(v.dx) > std::abs(v.dy)) {
                // Horizontal movement dominates
                a.row = (v.dx > 0) ? RIGHT : LEFT;  // right : left
            } else if (v.dy != 0) {
                // Vertical movement dominates
                a.row = (v.dy > 0) ? DOWN : UP;  // down : up
            }

            a.isPlaying = true;
            a.timer += dt;

            if (a.timer >= a.frameTime) {
                a.timer = 0.0f;
                a.currentFrame++;

                if (a.currentFrame > a.endFrame) {
                    a.currentFrame = a.startFrame;
                }
            }
        }
    }

    virtual ~Screen() = default;
};
