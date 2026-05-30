#pragma once

#include <iostream>
#include "entt/entt.hpp"

#include "defines/components/entityComponents.hpp"
#include "defines/components/combatComponents.hpp"
#include "defines/general.hpp"
#include <algorithm>

class Engine;

class Screen {

protected:
    entt::registry registry;
    entt::dispatcher dispatcher;
    Engine* engine = nullptr;

public:
    int width = 800;
    int height = 600;

    float scaleX = 1.0f;
    float scaleY = 1.0f;

    Screen() {
        width = GetScreenWidth();
        height = GetScreenHeight();
    };

    virtual void load(entt::registry& registry) = 0;
    virtual void update(float delta) = 0;
    virtual void draw() = 0;
    virtual void unload(entt::registry& registry) = 0;

    void setEngine(Engine* eng) { engine = eng; }
    Engine* getEngine() const { return engine; }

    entt::registry* getRegistryPtr() {
        return &registry;
    }

    entt::dispatcher* getDispatcherPtr() {
        return &dispatcher;
    }

    void basicUpdate(float delta) {
        updateScripts();
        updateAnimations(delta);

        auto view = registry.view<transform, velocity>();
        view.each([&](auto entity, transform &t, velocity &v) {
            // Update position
            t.position.x += v.dx;
            t.position.y += v.dy;
        });
    }

    void basicDraw() {
        auto view = registry.view<transform, sprite, animation>();
        view.each([&](auto entity, transform &t, sprite &s, animation &a) {

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

            auto hf = registry.try_get<hitFlash>(entity);
            Color filter = hf ? hf->filter : WHITE;
            // Draw single trail silhouette: chosen color + opacity + offset
            auto tr = registry.try_get<trail>(entity);
            if (tr && tr->enabled) {
                auto tex = s.textures.at(s.currentTexture);
                Rectangle trailDest = {
                    dest.x + tr->offset.x,
                    dest.y + tr->offset.y,
                    dest.width,
                    dest.height
                };
                ::Color tint = tr->color;
                tint.a = static_cast<unsigned char>(std::clamp(tr->alpha, 0.0f, 1.0f) * 255.0f);
                tex->Draw(source, trailDest, origin, t.rotation, tint);
            }

            s.textures.at(s.currentTexture)->Draw(source, dest, origin, t.rotation, filter);
        });

        auto scriptView = registry.view<script>();
        scriptView.each([&](auto entity, script &s) {
            if (s.instance) s.instance->onDraw();
        });
    }

    void basicUnload() {
        registry.clear();
    }

    void updateScripts() {
        float dt = GetFrameTime();
        auto scriptView = registry.view<script>();
        scriptView.each([&](auto entity, script &s) {
            if (s.instance) s.instance->onUpdate(dt);
        });
    }

    void updateAnimations(float dt) {
        auto view = registry.view<animation, velocity>();
        view.each([&](auto entity, animation &a, velocity &v) {
            if (!a.isPlaying) return;
            a.timer += dt;
            if (a.timer >= a.frameTime) {
                a.timer = 0.0f;
                a.currentFrame++;
                if (a.currentFrame > a.endFrame) a.currentFrame = a.startFrame;
            }
        });
    }

    virtual ~Screen() = default;
};
