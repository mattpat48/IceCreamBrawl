#include "raylib.h"

// NOTE: Gamepad name ID depends on drivers and OS
#define XBOX_ALIAS_1 "xbox"
#define XBOX_ALIAS_2 "x-box"
#define PS_ALIAS     "playstation"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
void handler()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);  // Set MSAA 4X hint before windows creation

    // Set axis deadzones
    const float leftStickDeadzoneX = 0.1f;
    const float leftStickDeadzoneY = 0.1f;
    const float rightStickDeadzoneX = 0.1f;
    const float rightStickDeadzoneY = 0.1f;
    const float leftTriggerDeadzone = -0.9f;
    const float rightTriggerDeadzone = -0.9f;
    //--------------------------------------------------------------------------------------

    int gamepad = 0; // which gamepad to display

    if (IsKeyPressed(KEY_LEFT) && gamepad > 0) { gamepad--; }
    if (IsKeyPressed(KEY_RIGHT)) { gamepad++; }

    if (IsGamepadAvailable(gamepad))
    {
        // Get axis values
        float leftStickX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X);
        float leftStickY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y);
        float rightStickX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_X);
        float rightStickY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_Y);
        float leftTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_TRIGGER);
        float rightTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER);

        // Calculate deadzones
        if (leftStickX > -leftStickDeadzoneX && leftStickX < leftStickDeadzoneX) { leftStickX = 0.0f; }
        if (leftStickY > -leftStickDeadzoneY && leftStickY < leftStickDeadzoneY) { leftStickY = 0.0f; }
        if (rightStickX > -rightStickDeadzoneX && rightStickX < rightStickDeadzoneX) { rightStickX = 0.0f; }
        if (rightStickY > -rightStickDeadzoneY && rightStickY < rightStickDeadzoneY) { rightStickY = 0.0f; }
        if (leftTrigger < leftTriggerDeadzone) { leftTrigger = -1.0f; }
        if (rightTrigger < rightTriggerDeadzone) { rightTrigger = -1.0f; }

        if (TextFindIndex(TextToLower(GetGamepadName(gamepad)), XBOX_ALIAS_1) > -1 || TextFindIndex(TextToLower(GetGamepadName(gamepad)), XBOX_ALIAS_2) > -1)
        {
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE)) { }

            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_RIGHT)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_LEFT)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_UP)) { }

            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP))  { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) { }

            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1)) { }

            Color leftGamepadColor = BLACK;
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_THUMB)) { }

            Color rightGamepadColor = BLACK;
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_THUMB)) { rightGamepadColor = RED; }
        }
        else if (TextFindIndex(TextToLower(GetGamepadName(gamepad)), PS_ALIAS) > -1)
        {
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE)) { }

            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_LEFT)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_RIGHT)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_UP)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) { }

            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) { }

            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1)) { }

            Color leftGamepadColor = BLACK;
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_THUMB)) { }

            Color rightGamepadColor = BLACK;
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_THUMB)) { }
        }
        else
        {
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_LEFT)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_RIGHT)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_UP)) { }

            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) { }

            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1)) { }

            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_THUMB)) { }

            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_THUMB)) { }

            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1)) { }
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1)) { }
        }

        for (int i = 0; i < GetGamepadAxisCount(0); i++)
        {
        }

        if (GetGamepadButtonPressed() != GAMEPAD_BUTTON_UNKNOWN) 
        {
        }
        else 
        {
        }
    }
    else
    {
    }
