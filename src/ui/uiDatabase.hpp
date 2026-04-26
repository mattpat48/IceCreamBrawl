#pragma once
#include "raylib.h"
#include <string>

struct ButtonStaticData {
    std::string texturePath;
    int spriteWidth;
    int spriteHeight;
    Vector2 scale;
    
    int animStartFrame;
    int animEndFrame;
    int animRow;
    float animFrameTime;

    // Posizione relativa allo schermo (es. 0.25f = 25% dello schermo)
    float relPosX;
    float relPosY;
};

struct JoystickStaticData {
    float relPosX;
    float relPosY;
    float radius;
};

class UIDatabase {
public:
    static ButtonStaticData getPrimaryAttackButtonData() {
        return {
            "resources/sprites/buttons/primary.png",
            220, 200,           // Dimensioni sprite
            {1.0f, 1.0f},     // Scala
            0, 2, 0, 0.1f,    // Animazione
            0.25f, 0.75f      // Posizione: 25% schermo orizzontale, 75% verticale
        };
    }

    static JoystickStaticData getJoystickData() {
        return { 0.75f, 0.75f, 200.0f }; // Posizione: 75% X, 75% Y, Raggio 200
    }

    static ButtonStaticData getDebuffAbilityButtonData() {
        return {
            "resources/sprites/buttons/primary.png",
            220, 200,
            {1.0f, 1.0f},
            0, 2, 0, 0.1f,
            0.40f, 0.75f
        };
    }

    static ButtonStaticData getBuffAbilityButtonData() {
        return {
            "resources/sprites/buttons/primary.png",
            220, 200,
            {1.0f, 1.0f},
            0, 2, 0, 0.1f,
            0.55f, 0.75f
        };
    }
};