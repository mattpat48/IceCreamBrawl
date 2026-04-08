#include "engine.hpp"

void Engine::run() {
	while (!WindowShouldClose()) {

		float delta = GetFrameTime();

		screenManager.update(delta);

		BeginDrawing();
		ClearBackground(LIGHTGRAY);

		DrawFPS(10, 10);
		screenManager.draw();

		EndDrawing();
	}
}