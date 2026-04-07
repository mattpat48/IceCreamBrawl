#pragma once

#include <entt/entt.hpp>
#include "raylib.h"
#include "engine/scripts/ui/controller/controller.hpp"
#include "defines/components/components.hpp"
#include "engine/assetManager.hpp"

class ControllerFactory {
public:

    static entt::entity createTouchJoystick(entt::registry& registry, AssetManager &assetManager,
                                          entt::entity playerEntity, 
                                          Vector2 position, float radius) {
        entt::entity joystick = registry.create();
        registry.emplace<is_ui>(joystick);

        std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> textures;
        textures["idle"] = assetManager.loadTexture(std::string("resources/sprites/controller/") + "handle.png");
        
        registry.emplace<sprite>(joystick, std::move(textures), "idle", 100, 100);
        registry.emplace<transform>(joystick, position, Vector2{1.0f, 1.0f}, 0.0f);
        registry.emplace<velocity>(joystick, 0.0f, 0.0f);
        registry.emplace<animation>(joystick, 0, 0, 0, 0, 0.1f, 0.0f, false, 0);
        
        registry.emplace<script>(joystick).bind<touchController>(joystick, registry, playerEntity, position, radius);
        return joystick;
    }
};
