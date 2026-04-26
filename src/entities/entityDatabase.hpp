#pragma once
#include "raylib.h"
#include <string>
#include "defines/components/entityComponents.hpp"

// Struttura che contiene tutti i dati STATICI di un'entità (grafica, collider, animazione base)
struct EntityStaticData {
    std::string basePath; // Cartella o prefisso per le texture
    int spriteWidth;
    int spriteHeight;
    Vector2 scale;
    
    // Animazione di base (es. Idle)
    int animStartFrame;
    int animEndFrame;
    int animRow;
    float animFrameTime;

    // Collider (Hitbox fisica indipendente dallo sprite)
    float colOffsetX;
    float colOffsetY;
    float colWidth;
    float colHeight;

    // Dati Logici e di Combattimento
    float maxHealth;
    float maxEndurance;
	float healthRegenRate; // Quantità di salute rigenerata ogni mezzo secondo
	float enduranceRegenRate; // Quantità di stamina rigenerata ogni mezzo secondo
    float baseDamage;
    float attackCost;
    float attackCooldown;
    float attackRange;
    AttackRange attackRangeType;
    AttackType attackType;
    AttackShape attackShape;
    float attackAngle;
};

class EntityDatabase {
public:
    static EntityStaticData getPlayerData() {
        return {
            "resources/sprites/player/", // basePath
            96, 80,                      // sprite Width, Height
            {6.0f, 6.0f},                // scale
            0, 7, 0, 0.1f,               // animazione (start, end, row, frameTime)
            -50.0f, -70.0f, 100.0f, 140.0f,  // collider (offsetX, offsetY, width, height)
            100.0f, 50.0f, 2.0f, 5.0f,	 // maxHealth, maxEndurance, healthRegenRate, enduranceRegenRate
			15.0f, 						 // baseDamage
            10.0f, 0.0f, 150.0f,         // attackCost, attackCooldown, attackRange
            AttackRange::MELEE, AttackType::AOE, AttackShape::CIRCLE, 0.0f // attack pattern
        };
    } 

    static EntityStaticData getEnemyData(const std::string& enemyType) {
        if (enemyType == "slime") {
            return {
                "resources/sprites/enemies/slime/", 
                64, 64, 
                {6.0f, 6.0f}, 
                0, 5, 0, 0.1f, 
                -40.0f, -40.0f, 80.0f, 80.0f,
                30.0f, 100.0f, 0.0f, 0.0f,
				20.0f,
                0.0f, 1.5f, 100.0f,
                AttackRange::MELEE, AttackType::TARGET, AttackShape::NONE, 0.0f
            };
        }
        // Fallback generico nel caso si sbagli a scrivere il nome
        return {
			"resources/sprites/enemies/default/", 
			64, 64, 
			{6.0f, 6.0f}, 
			0, 5, 0, 0.1f,
			-20.0f, 15.0f, 40.0f, 30.0f,
			30.0f, 100.0f, 0.0f, 0.0f,
			5.0f,
			0.0f, 1.5f, 100.0f,
			AttackRange::MELEE, AttackType::TARGET, AttackShape::NONE, 0.0f
		};
    }
};