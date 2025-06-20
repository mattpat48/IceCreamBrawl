#pragma once

#include "defines.h"
#include "scripts/script.hpp"
#include "entityComponents.hpp"

struct sprite {
    std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> textures;
    std::string currentTexture;
    int width;
    int height;
    Vector2 position;
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
    T& bind(Args&&... args) {
        instance = std::make_unique<T>(std::forward<Args>(args)...);
        instance->onCreate();
        return *static_cast<T*>(instance.get());
    }
};