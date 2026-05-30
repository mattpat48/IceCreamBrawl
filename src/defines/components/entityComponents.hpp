#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "entities/script.hpp"
#include "defines/general.hpp"

struct entityStatus {
    EntityStatus status;
};

struct sprite {
    std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> textures;
    std::unordered_map<std::string, std::string> texturePaths;
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
    bool dynamic = false;
    float dx;
    float dy;
    float destX;
    float destY;
    float maxSpeedX;
    float maxSpeedY;
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