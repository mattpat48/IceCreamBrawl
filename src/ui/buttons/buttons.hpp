#pragma once

#include "defines/components/components.hpp"
#include "input/inputSystem.hpp"
#include "raymath.h"

class buttonHandler : public Script {
public:

    void onCreate() override;
    void onDestroy() override;
    void onUpdate(float dt) override { }
    void onDraw() override { }

    virtual void buttonRelease() = 0;

    // Gestori degli eventi
    void onTouchDown(const TouchDownEvent& event);
    void onTouchMove(const TouchMoveEvent& event);
    void onTouchUp(const TouchUpEvent& event);

protected:
    bool isTracking = false;
    int activePointerId = -1;

    bool isPointInside(Vector2 point);
};