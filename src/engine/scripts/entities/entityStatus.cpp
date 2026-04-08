#include "entityStatus.hpp"

void entityStatus::onUpdate(float dt) {
	auto healthComp = getComponent<health>();
	auto enduranceComp = getComponent<endurance>();
	auto statusComp = getComponent<status>();
	auto spriteComp = getComponent<sprite>();
	auto animationComp = getComponent<animation>();
	auto attackComp = getComponent<attack>();

	APP_LOG("entityStatus updated: %d", statusComp->status);

	// 1. GESTIONE DELLA MORTE (Ha priorità assoluta)
	if (healthComp->life <= 0) {
		if (!statusComp->isDead()) {
			statusComp->status = DEAD;
			spriteComp->currentTexture = "death";
			animationComp->currentFrame = animationComp->startFrame;
			animationComp->isPlaying = true;
			APP_LOG("Entity is dead!");
		}
		// Ferma l'animazione di morte sull'ultimo frame
		else if (animationComp->isPlaying && animationComp->currentFrame == animationComp->endFrame) {
			animationComp->isPlaying = false;
		}
		return; // Se è morto, ignora tutto il resto!
	}

	// 2. MACCHINA A STATI PER LE ANIMAZIONI (State Machine)
	// Usiamo lo stato attuale per decidere che texture applicare
	if (statusComp->isAttacking()) {
		APP_LOG("Entity is ATTACK!");
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
		APP_LOG("Entity is RUN!");
		// Se il controller ha impostato RUNNING, assicuriamoci che la texture sia quella giusta
		if (spriteComp->currentTexture != "run") {
			spriteComp->currentTexture = "run"; // Assicurati di caricare "run.png" nella Factory!
			animationComp->currentFrame = animationComp->startFrame;
		}
	} 
	else if (statusComp->isIdle()) {
		APP_LOG("Entity is IDLE!");
		if (spriteComp->currentTexture != "idle") {
			spriteComp->currentTexture = "idle";
			animationComp->currentFrame = animationComp->startFrame;
		}
	}
}