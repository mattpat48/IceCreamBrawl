#include "engine.hpp"

void Engine::run() {
	while (!WindowShouldClose()) {

		float delta = GetFrameTime();

		screenManager.update(delta);

		BeginDrawing();
		ClearBackground(LIGHTGRAY);

		screenManager.draw();
		DrawFPS(10, 10);

		EndDrawing();
	}
}