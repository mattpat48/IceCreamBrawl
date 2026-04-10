#pragma once

// player sprite dimensions and paths
#define PLAYER_SPRITES_H_DIMENSION 96
#define PLAYER_SPRITES_V_DIMENSION 80
#define PLAYER_SPRITESHEET_ROWS 4
#define PLAYER_SPRITESHEET_COLUMNS 8
#define PLAYER_SPRITESHEET_ROW_DIMENSION (PLAYER_SPRITES_H_DIMENSION * PLAYER_SPRITESHEET_ROWS)
#define PLAYER_SPRITESHEET_COLUMN_DIMENSION (PLAYER_SPRITES_V_DIMENSION * PLAYER_SPRITESHEET_COLUMNS)
#define PLAYER_SPRITESHEET_DIMENSION (PLAYER_SPRITESHEET_ROW_DIMENSION * PLAYER_SPRITESHEET_COLUMN_DIMENSION)
#define PLAYER_SPRITESHEET_SIZE (PLAYER_SPRITESHEET_DIMENSION * PLAYER_SPRITESHEET_DIMENSION)
#define PLAYER_SPRITESHEET_PATH "resources/sprites/player/"

// Statistiche iniziali del player
struct PlayerBaseStats {
    static constexpr float MAX_HEALTH = 10.0f;
    static constexpr float REGEN_HEALTH = 2.0f;
    
    static constexpr float MAX_ENDURANCE = 100.0f;
    static constexpr float REGEN_ENDURANCE = 2.0f;
    
    static constexpr float ATTACK_COST = 10.0f;
    static constexpr float ATTACK_COOLDOWN = 0.5f;
    static constexpr float ATTACK_RANGE = 75.0f;

    static constexpr float BASE_DAMAGE = 1.0f;
};