#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

class Screen {
public:
    virtual void init() = 0;
    virtual void update(float delta) = 0;
    virtual void draw() = 0;
    virtual void unload() = 0;

    virtual ~Screen() = default;
};
