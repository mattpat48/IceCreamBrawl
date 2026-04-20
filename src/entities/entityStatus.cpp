#include "entityStatus.hpp"
#include "entt/entt.hpp"

static void setStatusWithEvent(entt::registry& registry, entt::entity entity, StatusType next, StatusChangeSource source) {
	auto* sta = registry.try_get<status>(entity);
	if (!sta) {
		return;
	}

	if (sta->status == next) {
		return;
	}

	const StatusType prev = sta->status;
	sta->status = next;
	registry.ctx().get<entt::dispatcher>().trigger(EntityStatusChangedEvent{entity, prev, next, source});
}

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
			animationComp->row = 0;
			spriteComp->currentTexture = "dead";
			animationComp->currentFrame = animationComp->startFrame;
			animationComp->isPlaying = true;
		} else if (animationComp->currentFrame == animationComp->endFrame) {
			handleDeath();
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
			setStatusWithEvent(*registry, entity, StatusType::IDLE, StatusChangeSource::EntityScript);
			spriteComp->currentTexture = "idle";
			attackComp->currentCooldown = attackComp->cooldown;
		}
	} 
	else if (statusComp->isRunning()) {
		// Se il controller ha impostato StatusType::RUNNING, assicuriamoci che la texture sia quella giusta
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
}