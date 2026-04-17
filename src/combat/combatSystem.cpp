#include "combatSystem.hpp"
#include "utils/logs.h"
#include <raymath.h>

static Vector2 getColliderCenter(entt::registry& registry, entt::entity e) {
    auto* t = registry.try_get<transform>(e);
    auto* c = registry.try_get<collider>(e);
    if (t && c) {
        return c->getCenter(t->position);
    } else if (t) {
        return t->position;
    }
    return {0.0f, 0.0f};
}

static Rectangle getColliderRect(entt::registry& registry, entt::entity e) {
    auto* t = registry.try_get<transform>(e);
    auto* c = registry.try_get<collider>(e);
    if (t && c) {
        return c->getRect(t->position);
    } else if (t) {
        return {t->position.x, t->position.y, 1.0f, 1.0f}; // Fallback
    }
    return {0.0f, 0.0f, 0.0f, 0.0f};
}

static bool isValidTarget(entt::registry& registry, entt::entity attacker, entt::entity victim) {
    if (!registry.valid(attacker) || !registry.valid(victim) || attacker == victim) {
        return false;
    }

    if (!registry.all_of<health, status>(victim)) {
        return false;
    }

    if (registry.get<status>(victim).isDead()) {
        return false;
    }

    // Evita il fuoco amico nemico-nemico mantenendo la logica gia' esistente.
    if (registry.any_of<is_enemy>(attacker) && registry.any_of<is_enemy>(victim)) {
        return false;
    }

    return true;
}

static entt::entity findClosestTargetInRange(entt::registry& registry, entt::entity attacker, float range) {
    entt::entity closestVictim = entt::null;
    float minDistance = range;
    const Vector2 attackerPos = getColliderCenter(registry, attacker);

    auto victims = registry.view<health, status>();
    for (auto victim : victims) {
        if (!isValidTarget(registry, attacker, victim)) {
            continue;
        }

        const float dist = Vector2Distance(attackerPos, getColliderCenter(registry, victim));
        if (dist <= minDistance) {
            minDistance = dist;
            closestVictim = victim;
        }
    }

    return closestVictim;
}

static bool isTargetInRange(entt::registry& registry, entt::entity attacker, entt::entity target, float range) {
    if (!isValidTarget(registry, attacker, target)) {
        return false;
    }

    const Vector2 attackerPos = getColliderCenter(registry, attacker);
    const Vector2 targetPos = getColliderCenter(registry, target);
    return Vector2Distance(attackerPos, targetPos) <= range;
}

static Vector2 getTargetArrowTip(entt::registry& registry, entt::entity target) {
    Rectangle rect = getColliderRect(registry, target);
    return {rect.x + (rect.width * 0.5f), rect.y - 6.0f};
}

void CombatSystem::update(entt::registry& registry, float dt) {
    // Troviamo tutti quelli che vogliono attaccare e hanno le statistiche di attacco
    auto attackers = registry.view<attack_intent, attack, endurance, status>();
    
    // 0. Aggiorniamo i timer dei feedback visivi
    auto feedbacks = registry.view<attack_feedback>();
    for (auto entity : feedbacks) {
        auto& fb = feedbacks.get<attack_feedback>(entity);
        fb.lifetime -= dt;
        if (fb.lifetime <= 0.0f) {
            registry.remove<attack_feedback>(entity);
        }
    }

    auto applyDamage = [&registry](entt::entity attacker, entt::entity victim) {
        float damageAmount = registry.get<damage>(attacker).currentDamage;
        if (auto* dmgRec = registry.try_get<damage_received>(victim)) {
            dmgRec->amount += damageAmount;
        } else {
            registry.emplace<damage_received>(victim, damageAmount);
        }
        APP_LOG("Hit! Attacker %d hits Victim %d for %.2f damage!", attacker, victim, damageAmount);
    };

    // Mantiene aggiornato il lock del target per il player quando usa attacco TARGET.
    auto targeters = registry.view<is_player, attack, status>();
    for (auto entity : targeters) {
        auto& atk = targeters.get<attack>(entity);
        auto& sta = targeters.get<status>(entity);

        if (sta.isDead() || atk.type != AttackType::TARGET) {
            if (registry.any_of<target_lock>(entity)) {
                registry.remove<target_lock>(entity);
            }
            continue;
        }

        entt::entity currentTarget = entt::null;
        if (auto* lock = registry.try_get<target_lock>(entity)) {
            currentTarget = lock->target;
        }

        if (!isTargetInRange(registry, entity, currentTarget, atk.range)) {
            currentTarget = findClosestTargetInRange(registry, entity, atk.range);
        }

        if (currentTarget == entt::null) {
            if (registry.any_of<target_lock>(entity)) {
                registry.remove<target_lock>(entity);
            }
        } else {
            registry.emplace_or_replace<target_lock>(entity, currentTarget);
        }
    }

    for (auto attacker : attackers) {
        auto& atk = attackers.get<attack>(attacker);
        auto& end = attackers.get<endurance>(attacker);
        auto& sta = attackers.get<status>(attacker);

        if (atk.canAttack(end.stamina) && !sta.isDead() && !sta.isAttacking()) {
            sta.status = StatusType::ATTACK; // Imposta lo stato su StatusType::ATTACK
            end.consume(atk.cost);
            atk.currentCooldown = atk.cooldown;

            auto victims = registry.view<health, status>();
            
            Vector2 atkPos = getColliderCenter(registry, attacker);

            // --- 1. ATTACCO A TARGET (Cerca il più vicino) ---
            if (atk.type == AttackType::TARGET) {
                entt::entity selectedTarget = entt::null;

                if (registry.any_of<is_player>(attacker)) {
                    if (auto* lock = registry.try_get<target_lock>(attacker)) {
                        if (isTargetInRange(registry, attacker, lock->target, atk.range)) {
                            selectedTarget = lock->target;
                        }
                    }
                }

                if (selectedTarget == entt::null) {
                    selectedTarget = findClosestTargetInRange(registry, attacker, atk.range);

                    if (registry.any_of<is_player>(attacker) && selectedTarget != entt::null) {
                        registry.emplace_or_replace<target_lock>(attacker, selectedTarget);
                    }
                }

                if (selectedTarget != entt::null) {
                    applyDamage(attacker, selectedTarget);
                    
                    // Creiamo il feedback visivo sul bersaglio
                    registry.emplace_or_replace<attack_feedback>(attacker, 
                        0.2f, 0.2f, AttackType::TARGET, AttackShape::NONE, 0.0f, 0.0f, atkPos, Vector2{0,0}, selectedTarget);
                }
            } 
            // --- 2. ATTACCO AD AREA (Colpisce tutti nella forma) ---
            else if (atk.type == AttackType::AOE) {
                // Mappiamo la direzione se ci serve per un cono o una linea
                Vector2 facingDir = {0.0f, 1.0f}; // Default giù
                if (auto* anim = registry.try_get<animation>(attacker)) {
                    if (anim->direction == Directions::RIGHT)      facingDir = { 1.0f,  0.0f};
                    else if (anim->direction == Directions::LEFT)  facingDir = {-1.0f,  0.0f};
                    else if (anim->direction == Directions::UP)    facingDir = { 0.0f, -1.0f};
                    else if (anim->direction == Directions::DOWN)  facingDir = { 0.0f,  1.0f};
                }

                for (auto victim : victims) {
                    if (attacker == victim || (registry.any_of<is_enemy>(attacker) && registry.any_of<is_enemy>(victim))) continue;
                    
                    Rectangle vicRect = getColliderRect(registry, victim);
                    Vector2 vicCenter = getColliderCenter(registry, victim);
                    float dist = Vector2Distance(atkPos, vicCenter);

                    bool hit = false;
                    if (atk.shape == AttackShape::CIRCLE) {
                        hit = CheckCollisionCircleRec(atkPos, atk.range, vicRect);
                    } 
                    else if (atk.shape == AttackShape::CONE) {
                        // Prima controlliamo se il rettangolo è almeno nel raggio del cono (Cerchio-Rettangolo)
                        if (CheckCollisionCircleRec(atkPos, atk.range, vicRect)) {
                            if (dist <= 0.1f) { 
                                hit = true; // Perfettamente sovrapposti
                            } else {
                                Vector2 toVictim = Vector2Normalize(Vector2Subtract(vicCenter, atkPos));
                                float dotProduct = Vector2DotProduct(facingDir, toVictim);
                                float threshold = cosf((atk.angle / 2.0f) * DEG2RAD);
                                if (dotProduct >= threshold) hit = true;
                            }
                        }
                    }

                    if (hit) applyDamage(attacker, victim);
                }
                
                // Creiamo il feedback visivo dell'area (indipendentemente se abbiamo colpito qualcuno o no)
                registry.emplace_or_replace<attack_feedback>(attacker, 
                    0.2f, 0.2f, AttackType::AOE, atk.shape, atk.angle, atk.range, atkPos, facingDir, entt::null);
            }
        }
        // Rimuoviamo l'intento di attaccare, l'azione è stata processata
        registry.remove<attack_intent>(attacker);
    }

    auto cooldowners = registry.view<attack>();
    for (auto entity : cooldowners) {
        auto& atk = cooldowners.get<attack>(entity);
        atk.updateCooldown(dt);
    }
}

void CombatSystem::draw(entt::registry& registry) {
    auto targetLocks = registry.view<is_player, attack, status, target_lock>();
    for (auto entity : targetLocks) {
        auto& atk = targetLocks.get<attack>(entity);
        auto& sta = targetLocks.get<status>(entity);
        auto& lock = targetLocks.get<target_lock>(entity);

        if (sta.isDead() || atk.type != AttackType::TARGET) {
            continue;
        }

        if (!isTargetInRange(registry, entity, lock.target, atk.range)) {
            continue;
        }

        const Vector2 tip = getTargetArrowTip(registry, lock.target);
        const float t = static_cast<float>(GetTime());
        const float pulse = 1.0f + (sinf(t * 6.0f) * 0.18f);
        const float halfWidth = 13.0f * pulse;
        const float height = 24.0f * pulse;

        const Vector2 left = {tip.x - halfWidth, tip.y - height};
        const Vector2 right = {tip.x + halfWidth, tip.y - height};
        const Color fill = {255, 36, 36, 255};
        const Color border = {255, 240, 240, 255};

        DrawTriangle(left, right, tip, fill);
        DrawLineEx(left, right, 3.0f, border);
        DrawLineEx(right, tip, 3.0f, border);
        DrawLineEx(tip, left, 3.0f, border);
    }

    auto feedbacks = registry.view<attack_feedback>();
    
    for (auto entity : feedbacks) {
        auto& fb = feedbacks.get<attack_feedback>(entity);
        
        // Calcoliamo la trasparenza (fade out)
        float alpha = fb.lifetime / fb.maxLifetime;
        Color fillColor = Fade(GRAY, alpha * 0.5f); // Grigio semi-trasparente
        Color outlineColor = Fade(DARKGRAY, alpha);
        
        if (fb.type == AttackType::AOE) {
            if (fb.shape == AttackShape::CIRCLE) {
                DrawCircleV(fb.origin, fb.range, fillColor);
                DrawCircleLines(fb.origin.x, fb.origin.y, fb.range, outlineColor);
            } 
            else if (fb.shape == AttackShape::CONE) {
                // Calcoliamo l'angolo centrale in gradi partendo dal vettore di direzione
                float centerAngle = atan2(fb.direction.y, fb.direction.x) * RAD2DEG;
                float startAngle = centerAngle - (fb.angle / 2.0f);
                float endAngle = centerAngle + (fb.angle / 2.0f);
                
                DrawCircleSector(fb.origin, fb.range, startAngle, endAngle, 36, fillColor);
                DrawCircleSectorLines(fb.origin, fb.range, startAngle, endAngle, 36, outlineColor);
            }
        } 
        else if (fb.type == AttackType::TARGET && registry.valid(fb.target)) {
            // Disegniamo un "mirino" rosso sotto i piedi o sopra la testa del bersaglio
            Vector2 targetPos = getColliderCenter(registry, fb.target);
            Color targetColor = Fade(RED, alpha);
            
            DrawCircleLines(targetPos.x, targetPos.y, 25.0f, targetColor);
            DrawLine(targetPos.x - 35, targetPos.y, targetPos.x + 35, targetPos.y, targetColor);
            DrawLine(targetPos.x, targetPos.y - 35, targetPos.x, targetPos.y + 35, targetColor);
        }
    }

    // DEBUG: Disegniamo tutte le hitbox verdi per farti sistemare i valori nei Factory!
    auto colliders = registry.view<transform, collider>();
    for (auto entity : colliders) {
        auto& t = colliders.get<transform>(entity);
        auto& c = colliders.get<collider>(entity);
        Rectangle rect = c.getRect(t.position);
        
        DrawRectangleLinesEx(rect, 2.0f, GREEN);
        
        // Disegna anche un puntino al centro del collider
        Vector2 center = c.getCenter(t.position);
        DrawCircleV(center, 3.0f, BLUE);
    }
}
