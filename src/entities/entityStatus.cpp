#include "entityStatus.hpp"

void entityStatus::onUpdate(float dt) {
	auto healthComp = getComponent<health>();
	auto enduranceComp = getComponent<endurance>();
	auto statusComp = getComponent<status>();
	auto spriteComp = getComponent<sprite>();
	auto animationComp = getComponent<animation>();
	auto attackComp = getComponent<attack>();

	// 1. GESTIONE DELLA MORTE (Ha priorità assoluta)
	if (statusComp->isDead()) {
		if (spriteComp->currentTexture != "dead") {
			spriteComp->currentTexture = "dead"; // Assicurati di avere una texture "dead.png" caricata nella Factory!
			animationComp->currentFrame = animationComp->startFrame;
		} else if (animationComp->currentFrame == animationComp->endFrame) {
			registry->destroy(entity); // Se vuoi distruggerla subito
		}
		return; // Se è morto, non facciamo nient'altro
	}

	// 2. MACCHINA A STATI PER LE ANIMAZIONI (State Machine)
	// Usiamo lo stato attuale per decidere che texture applicare
	if (statusComp->isAttacking()) {
		if (spriteComp->currentTexture != "attack") {
			spriteComp->currentTexture = "attack";
			animationComp->currentFrame = animationComp->startFrame;
		} else if (animationComp->currentFrame == animationComp->endFrame) {
			statusComp->status = IDLE;
			spriteComp->currentTexture = "idle";
			attackComp->currentCooldown = attackComp->cooldown;
		}
	} 
	else if (statusComp->isRunning()) {
		// Se il controller ha impostato RUNNING, assicuriamoci che la texture sia quella giusta
		if (spriteComp->currentTexture != "run") {
			spriteComp->currentTexture = "run"; // Assicurati di caricare "run.png" nella Factory!
			animationComp->currentFrame = animationComp->startFrame;
		}
	} 
	else if (statusComp->isIdle()) {
		if (spriteComp->currentTexture != "idle") {
			spriteComp->currentTexture = "idle";
			animationComp->currentFrame = animationComp->startFrame;
		}
	}
}

void entityStatus::onDraw() {
	auto healthComp = getComponent<health>();
	auto enduranceComp = getComponent<endurance>();

	if (healthComp) {
		float healthPercent = healthComp->life / healthComp->maxLife;
		DrawRectangle(10, 40, 200 * healthPercent, 20, RED);
		DrawRectangleLines(10, 40, 200, 20, BLACK);
	}

	if (enduranceComp) {
		float staminaPercent = enduranceComp->stamina / enduranceComp->maxStamina;
		DrawRectangle(10, 70, 200 * staminaPercent, 20, BLUE);
		DrawRectangleLines(10, 70, 200, 20, BLACK);
	}
}