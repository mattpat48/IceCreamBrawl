#include "minimap.hpp"

void minimap::onDraw() {
	// Controlla che l'entità del giocatore sia ancora valida
	if (!registry->valid(playerEntity)) return;

	auto* pTransform = registry->try_get<transform>(playerEntity);
	if (!pTransform) return;

	// 1. Definisci le dimensioni della minimappa
	float minimapWidth = 200.0f;
	// Calcola l'altezza mantenendo le stesse proporzioni della mappa globale
	float minimapHeight = minimapWidth * (mapHeight / mapWidth);
	
	float margin = 20.0f;
	float screenWidth = static_cast<float>(GetScreenWidth());
	
	// 2. Posiziona la minimappa in alto a destra
	Rectangle minimapRect = {
		10.0f,
		100.0f,
		minimapWidth,
		minimapHeight
	};

	// 3. Disegna lo sfondo e il bordo
	DrawRectangleRec(minimapRect, Fade(LIGHTGRAY, 0.6f)); // Sfondo semitrasparente
	DrawRectangleLinesEx(minimapRect, 3.0f, BLACK);       // Bordo nero

	// 4. Calcola la posizione del player sulla minimappa
	// Clamp previene che il punto rosso esca dai bordi della minimappa se il player sbattesse ai limiti del mondo
	float playerMapX = Clamp(pTransform->position.x, 0.0f, mapWidth);
	float playerMapY = Clamp(pTransform->position.y, 0.0f, mapHeight);

	// Calcolo della proporzione rispetto alla risoluzione della minimappa
	float playerMinimapX = minimapRect.x + (playerMapX / mapWidth) * minimapWidth;
	float playerMinimapY = minimapRect.y + (playerMapY / mapHeight) * minimapHeight;

	// 5. Disegna il punto rosso per il giocatore
	DrawCircle(static_cast<int>(playerMinimapX), static_cast<int>(playerMinimapY), 5.0f, RED);

	// 6. Mostra le coordinate del player sulla minimappa (opzionale, per debug)
	std::string coordsText = "X: " + std::to_string(static_cast<int>(pTransform->position.x)) + 
							 " Y: " + std::to_string(static_cast<int>(pTransform->position.y));
	DrawText(coordsText.c_str(), static_cast<int>(minimapRect.x), static_cast<int>(minimapRect.y + minimapRect.height + 5), 30, BLACK);
}