#pragma once

#include "raylib.h"

struct TapEvent {
    Vector2 position; // position of the tap
    float duration;   // duration in seconds
    int pointerId;    // mouse or touch id
};

struct LongTapEvent {
    Vector2 position; // position where long tap occurred
    float duration;   // duration in seconds
    int pointerId;
};

struct SwipeEvent {
    Vector2 start;    // start position
    Vector2 end;      // end position
    Vector2 delta;    // end - start
    float duration;   // duration in seconds
    int pointerId;
};