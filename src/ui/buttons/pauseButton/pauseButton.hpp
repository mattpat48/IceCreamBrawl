#pragma once

#include "engine/script.hpp"
#include "input/inputSystem.hpp"
#include "defines/events.hpp"

class PauseButton : public Script {
public:
    PauseButton() = default;
    ~PauseButton() override = default;

    void onCreate() override;
    void onDestroy() override;

    void onTouchDown(const TouchDownEvent& event);

private:
    Rectangle bounds;
};