#pragma once
#include "raylib.h"
#include "raylib-cpp.hpp"

struct sprite {
    raylib::Texture2D texture;
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