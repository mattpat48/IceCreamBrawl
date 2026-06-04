#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"
#include "entt/entt.hpp"
#include <vector>

#include "defines/general.hpp"
#include "entities/script.hpp"
#include "defines/events/gameEvents.hpp"
#include "entities/entityInfos.hpp"
#include "engine/movementManager.hpp"

#include "utils/log.h"

class grid {
public:

	grid() : registryPtr(nullptr), dispatcherPtr(nullptr) {
		for (int r = 0; r < rows; ++r)
			for (int c = 0; c < columns; ++c)
				for (int k = 0; k < 3; ++k)
					gridCells[r][c][k] = entt::null;
	}

	void load(entt::registry& registry, entt::dispatcher& dispatcher, MovementManager& moveMgr) {
		registryPtr = &registry;
		dispatcherPtr = &dispatcher;
		moveManager = &moveMgr;

		dispatcherPtr->sink<PlayerMovementEvent>().connect<&grid::playerMovement>(this);
	}

	std::vector<entt::entity> getCellEntities(int r, int c) {
		if (r < 0 || r >= rows || c < 0 || c >= columns) return {};
		std::vector<entt::entity> out;
		out.reserve(3);
		for (int k = 0; k < 3; ++k)
			if (gridCells[r][c][k] != entt::null) out.push_back(gridCells[r][c][k]);
		return out;
	}

	bool addToCell(int r, int c, entt::entity e) {
		if (r < 0 || r >= rows || c < 0 || c >= columns) return false;
		if (e == entt::null) return false;
		if (!registryPtr) return false;

		// get incoming exclusivity
		auto incomingInfo = registryPtr->try_get<gridInfo>(e);
		bool incomingExclusive = incomingInfo ? incomingInfo->isExclusive : false;

		// check existing occupants for exclusivity conflicts and find free slot
		int freeSlot = -1;
		for (int k = 0; k < 3; ++k) {
			auto occupant = gridCells[r][c][k];
			if (occupant == entt::null) {
				if (freeSlot == -1) freeSlot = k;
				continue;
			}
			if (!registryPtr->valid(occupant)) continue;
			auto occInfo = registryPtr->try_get<gridInfo>(occupant);
			bool occExclusive = occInfo ? occInfo->isExclusive : false;

			// if incoming is exclusive, cell must be empty
			if (incomingExclusive) return false;

			// if existing occupant is exclusive, cannot add non-exclusive
			if (occExclusive) return false;
		}

		if (freeSlot == -1) return false; // no space

		gridCells[r][c][freeSlot] = e;
		return true;
	}

	void setCell(int r, int c, entt::entity e) {
		if (r < 0 || r >= rows || c < 0 || c >= columns) return;
		if (e == entt::null) {
			for (int k = 0; k < 3; ++k) gridCells[r][c][k] = entt::null;
			return;
		}
		if (!registryPtr) return;

		auto incomingInfo = registryPtr->try_get<gridInfo>(e);
		bool incomingExclusive = incomingInfo ? incomingInfo->isExclusive : false;

		// if incoming is exclusive, clear cell and set it as sole occupant
		if (incomingExclusive) {
			for (int k = 0; k < 3; ++k) gridCells[r][c][k] = entt::null;
			gridCells[r][c][0] = e;
			return;
		}

		// if there's any exclusive occupant, do not overwrite
		for (int k = 0; k < 3; ++k) {
			auto occupant = gridCells[r][c][k];
			if (occupant == entt::null) continue;
			if (!registryPtr->valid(occupant)) continue;
			auto occInfo = registryPtr->try_get<gridInfo>(occupant);
			if (occInfo && occInfo->isExclusive) return;
		}

		// otherwise set at index 0 (replacing whatever was there)
		gridCells[r][c][0] = e;
	}

	bool removeFromCell(int r, int c, entt::entity e) {
		if (r < 0 || r >= rows || c < 0 || c >= columns) return false;
		for (int k = 0; k < 3; ++k) {
			if (gridCells[r][c][k] == e) {
				// shift left
				for (int j = k; j < 2; ++j) gridCells[r][c][j] = gridCells[r][c][j+1];
				gridCells[r][c][2] = entt::null;
				return true;
			}
		}
		return false;
	}

	void clearCell(int r, int c) {
		if (r < 0 || r >= rows || c < 0 || c >= columns) return;
		for (int k = 0; k < 3; ++k) gridCells[r][c][k] = entt::null;
	}

	bool moveEntity(int fromRow, int fromColumn, int toRow, int toColumn, entt::entity movingEntity) {
		if (fromRow < 0 || fromRow >= rows || fromColumn < 0 || fromColumn >= columns) return false;
		if (toRow < 0 || toRow >= rows || toColumn < 0 || toColumn >= columns) return false;
		if (movingEntity == entt::null || !registryPtr || !registryPtr->valid(movingEntity)) return false;
		bool foundInSource = false;
		for (int k = 0; k < 3; ++k) {
			if (gridCells[fromRow][fromColumn][k] == movingEntity) {
				foundInSource = true;
				break;
			}
		}
		if (!foundInSource) return false;
		ICB_LOGI("Found entity %d in source cell (%d, %d)", int(movingEntity), fromRow, fromColumn);
		// controlla se destinazione rispetta le regole di esclusività e se ha spazio
		auto statusComp = registryPtr->try_get<entityStatus>(movingEntity);
		if (statusComp) {
			if (statusComp->status != EntityStatus::IDLE)
				return false;
		}
		auto movingInfo = registryPtr->try_get<gridInfo>(movingEntity);
		bool movingExclusive = movingInfo ? movingInfo->isExclusive : false;
		int freeSlot = -1;
		for (int k = 0; k < 3; ++k) {
			auto occupant = gridCells[toRow][toColumn][k];
			if (occupant == entt::null) { if (freeSlot == -1) freeSlot = k; continue; }
			if (!registryPtr->valid(occupant)) continue;
			auto occInfo = registryPtr->try_get<gridInfo>(occupant);
			bool occExclusive = occInfo ? occInfo->isExclusive : false;
			// if moving entity is exclusive, destination must be empty
			if (movingExclusive) return false;
			// if destination has an exclusive occupant, cannot move non-exclusive into it
			if (occExclusive) return false;
		}
		if (freeSlot == -1) return false;

		// rimuovi dalla sorgente e aggiungi alla destinazione
		if (!removeFromCell(fromRow, fromColumn, movingEntity)) return false; // dovrebbe succedere
		if (!addToCell(toRow, toColumn, movingEntity)) {
			// rollback: reinserisci nella sorgente
			addToCell(fromRow, fromColumn, movingEntity);
			return false;
		}

		auto entityPos = registryPtr->try_get<gridInfo>(movingEntity);
		if (entityPos) {
			entityPos->row = static_cast<Rows>(toRow);
			entityPos->column = static_cast<Columns>(toColumn);
		}

		auto targetCenter = EntityDatabase::getInstance().getCellCenter(static_cast<Rows>(toRow), static_cast<Columns>(toColumn));
		if (moveManager) moveManager->handleDynamicScrollProfile(movingEntity, *registryPtr, targetCenter, Vector2{GetScreenWidth() / 20.0f, GetScreenHeight() / 20.0f}, MovementManager::MovementProfile::EaseOut, 0.4f);

		ICB_LOGI("Moved entity %d from (%d, %d) to (%d, %d)", int(movingEntity), fromRow, fromColumn, toRow, toColumn);
		
		return true;
	}
	
	bool playerMovement(PlayerMovementEvent& e) {
		ICB_LOGI("Received PlayerMovementEvent for entity %d: direction %d, origin (%d, %d)", int(e.playerEntity), static_cast<int>(e.direction), static_cast<int>(e.originRow), static_cast<int>(e.originColumn));
		if (e.handled) return false;
		e.handled = true;

		return moveEntity(static_cast<int>(e.originRow), static_cast<int>(e.originColumn), 
			e.direction == Directions::UP ? static_cast<int>(e.originRow) - 1 : (e.direction == Directions::DOWN ? static_cast<int>(e.originRow) + 1 : static_cast<int>(e.originRow)),
			e.direction == Directions::LEFT ? static_cast<int>(e.originColumn) - 1 : (e.direction == Directions::RIGHT ? static_cast<int>(e.originColumn) + 1 : static_cast<int>(e.originColumn)),
			e.playerEntity);
	}

protected:
	int rows = GridVariables::GRID_ROWS;
	int columns = GridVariables::GRID_COLUMNS;

	entt::dispatcher* dispatcherPtr = nullptr; // da settare esternamente se serve
	entt::registry* registryPtr = nullptr; // da settare esternamente se serve
	MovementManager* moveManager = nullptr; // da settare esternamente se serve

	entt::entity gridCells[GridVariables::GRID_ROWS][GridVariables::GRID_COLUMNS][3] = {entt::null};
};