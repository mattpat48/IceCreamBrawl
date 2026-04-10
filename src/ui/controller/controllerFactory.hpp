#pragma once

#include <entt/entt.hpp>
#include "raylib.h"
#include "ui/controller/controller.hpp"
#include "defines/components/components.hpp"
#include "engine/assetManager.hpp"

class ControllerFactory {
public:

    static entt::entity createTouchJoystick(entt::registry& registry, AssetManager &assetManager,
                                          entt::entity playerEntity, 
                                          Vector2 position, float radius) {
        entt::entity handle = registry.create();
        registry.emplace<is_ui>(handle);

        std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> textures;
        textures["idle"] = assetManager.loadTexture(std::string("resources/sprites/controller/") + "handle.png");
        
        registry.emplace<sprite>(handle, std::move(textures), "idle", 100, 100);
        registry.emplace<transform>(handle, position, Vector2{1.0f, 1.0f}, 0.0f);
        registry.emplace<velocity>(handle, 0.0f, 0.0f);
        registry.emplace<animation>(handle, 0, 0, 0, 0, 0.1f, 0.0f, false, 0);
        
        registry.emplace<script>(handle).bind<touchController>(handle, registry, playerEntity, position, radius);

        entt::entity circle = registry.create();
        registry.emplace<is_ui>(circle);

        std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> circleTextures;
        circleTextures["idle"] = assetManager.loadTexture(std::string("resources/sprites/controller/") + "joystick.png");

        registry.emplace<sprite>(circle, std::move(circleTextures), "idle", 300, 300);
        registry.emplace<transform>(circle, position, Vector2{1.0f, 1.0f}, 0.0f);
        registry.emplace<velocity>(circle, 0.0f, 0.0f);
        registry.emplace<animation>(circle, 0, 0, 0, 0, 0.1f, 0.0f, false, 0);

        return handle;
    }
};
