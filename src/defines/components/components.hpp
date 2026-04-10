#pragma once

#include "defines/defines_screen.h"
#include "defines/defines_player.h"
#include "defines/defines_ui.h"
#include "defines/defines_general.h"
#include "engine/script.hpp"
#include "components/entityComponents.hpp"

struct is_ui {
    
};

struct is_map_background {
    bool tiled;
};

struct sprite {
    std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> textures;
    std::string currentTexture;
    int width;
    int height;
};

struct transform {
    raylib::Vector2 position;
    raylib::Vector2 scale;
    float rotation;
};

struct velocity {
    float dx;
    float dy;
};

struct animation {
    int currentFrame;
    int startFrame;
    int endFrame;
    int row;
    float frameTime;
    float timer;
    bool isPlaying;
    int direction;
};

struct script {
    std::unique_ptr<Script> instance = nullptr;

    template <typename T, typename... Args>
    T& bind(entt::entity ownerEntity, entt::registry& reg, Args&&... args) {
        instance = std::make_unique<T>(std::forward<Args>(args)...);
        instance->entity = ownerEntity;
        instance->registry = &reg;
        instance->onCreate();
        return *static_cast<T*>(instance.get());
    }
};