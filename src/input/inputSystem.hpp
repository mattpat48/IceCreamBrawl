#pragma once
#include <entt/entt.hpp>
#include <raylib.h>
#include <raymath.h>
#include <unordered_map>

// 1. Definiamo gli Eventi che l'Input System "lancerà" al resto del gioco.
// I componenti (come il touchController) ascolteranno questi eventi.

struct TouchDownEvent {
    Vector2 position;
    int pointerId;
    mutable bool handled = false; // Flag per il consumo dell'evento
};

struct TouchUpEvent {
    Vector2 position;
    int pointerId;
    mutable bool handled = false;
};

struct TouchMoveEvent {
    Vector2 position;
    Vector2 delta;
    int pointerId;
    mutable bool handled = false;
};

struct GestureEvent {
    int gesture; // Costanti di Raylib es. GESTURE_SWIPE_RIGHT, GESTURE_TAP
    Vector2 position;
    mutable bool handled = false;
};

// 2. La classe InputSystem vera e propria
class InputSystem {
private:
    entt::dispatcher& dispatcher; // Riferimento al dispatcher globale

    std::unordered_map<int, Vector2> activePointers;

public:
    InputSystem(entt::dispatcher& disp);
    
    void update();
};