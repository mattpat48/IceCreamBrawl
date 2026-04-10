#pragma once

#include <iostream>
#include <memory>

#include "defines/components/components.hpp"
#include <entt/entt.hpp>
#include <raymath.h>

#include "input/inputSystem.hpp"

class Engine;

class Screen {

protected:
    entt::registry registry;
    Engine* engine = nullptr;

    std::unique_ptr<InputSystem> inputSystem;

public:
    int width;
    int height;

    float scaleX = 1.0f;
    float scaleY = 1.0f;

    Camera2D camera = { 0 };

    Screen() {
        width = GetScreenWidth();
        height = GetScreenHeight();
        
        // Inizializza la camera per centrare il target al centro dello schermo
        camera.target = { 0.0f, 0.0f };
        camera.offset = { width / 2.0f, height / 2.0f };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;

        registry.ctx().emplace<entt::dispatcher>();
        inputSystem = std::make_unique<InputSystem>(registry.ctx().get<entt::dispatcher>());
    };

    virtual void load(entt::registry& registry) = 0;
    virtual void update(float delta) = 0;
    virtual void draw() = 0;
    virtual void unload(entt::registry& registry) = 0;

    void setEngine(Engine* eng) { engine = eng; }
    Engine* getEngine() const { return engine; }

    void DrawTextureTiled(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, float scale, Color tint);

    void basicUpdate(float delta);
    void basicDraw();
    void basicUnload();

    void updateScripts();
    void updateAnimations(float dt);

    virtual ~Screen() = default;
};
