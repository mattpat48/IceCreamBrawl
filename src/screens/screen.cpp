#include "screen.hpp"

void Screen::DrawTextureTiled(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, float scale, Color tint) {
	if ((texture.id <= 0) || (scale <= 0.0f)) return;  // Wanna see a infinite loop?!...just delete this line!
	if ((source.width == 0) || (source.height == 0)) return;

	int tileWidth = (int)(source.width*scale), tileHeight = (int)(source.height*scale);
	if ((dest.width < tileWidth) && (dest.height < tileHeight))
	{
		// Can fit only one tile
		DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)dest.width/tileWidth)*source.width, ((float)dest.height/tileHeight)*source.height},
					(Rectangle){dest.x, dest.y, dest.width, dest.height}, origin, rotation, tint);
	}
	else if (dest.width <= tileWidth)
	{
		// Tiled vertically (one column)
		int dy = 0;
		for (;dy+tileHeight < dest.height; dy += tileHeight)
		{
			DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)dest.width/tileWidth)*source.width, source.height}, (Rectangle){dest.x, dest.y + dy, dest.width, (float)tileHeight}, origin, rotation, tint);
		}

		// Fit last tile
		if (dy < dest.height)
		{
			DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)dest.width/tileWidth)*source.width, ((float)(dest.height - dy)/tileHeight)*source.height},
						(Rectangle){dest.x, dest.y + dy, dest.width, dest.height - dy}, origin, rotation, tint);
		}
	}
	else if (dest.height <= tileHeight)
	{
		// Tiled horizontally (one row)
		int dx = 0;
		for (;dx+tileWidth < dest.width; dx += tileWidth)
		{
			DrawTexturePro(texture, (Rectangle){source.x, source.y, source.width, ((float)dest.height/tileHeight)*source.height}, (Rectangle){dest.x + dx, dest.y, (float)tileWidth, dest.height}, origin, rotation, tint);
		}

		// Fit last tile
		if (dx < dest.width)
		{
			DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)(dest.width - dx)/tileWidth)*source.width, ((float)dest.height/tileHeight)*source.height},
						(Rectangle){dest.x + dx, dest.y, dest.width - dx, dest.height}, origin, rotation, tint);
		}
	}
	else
	{
		// Tiled both horizontally and vertically (rows and columns)
		int dx = 0;
		for (;dx+tileWidth < dest.width; dx += tileWidth)
		{
			int dy = 0;
			for (;dy+tileHeight < dest.height; dy += tileHeight)
			{
				DrawTexturePro(texture, source, (Rectangle){dest.x + dx, dest.y + dy, (float)tileWidth, (float)tileHeight}, origin, rotation, tint);
			}

			if (dy < dest.height)
			{
				DrawTexturePro(texture, (Rectangle){source.x, source.y, source.width, ((float)(dest.height - dy)/tileHeight)*source.height},
					(Rectangle){dest.x + dx, dest.y + dy, (float)tileWidth, dest.height - dy}, origin, rotation, tint);
			}
		}

		// Fit last column of tiles
		if (dx < dest.width)
		{
			int dy = 0;
			for (;dy+tileHeight < dest.height; dy += tileHeight)
			{
				DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)(dest.width - dx)/tileWidth)*source.width, source.height},
						(Rectangle){dest.x + dx, dest.y + dy, dest.width - dx, (float)tileHeight}, origin, rotation, tint);
			}

			// Draw final tile in the bottom right corner
			if (dy < dest.height)
			{
				DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)(dest.width - dx)/tileWidth)*source.width, ((float)(dest.height - dy)/tileHeight)*source.height},
					(Rectangle){dest.x + dx, dest.y + dy, dest.width - dx, dest.height - dy}, origin, rotation, tint);
			}
		}
	}
}

void Screen::basicUpdate(float delta) {
	// 1. Legge gli input da Raylib e lancia gli eventi tramite il dispatcher
	if (inputSystem) {
		inputSystem->update();
	}

	updateScripts();
	updateAnimations(delta);

	auto view = registry.view<transform, velocity>();
	for (auto entity : view) {
		auto& t = view.get<transform>(entity);
		auto& v = view.get<velocity>(entity);

		// Update position
		t.position.x += v.dx;
		t.position.y += v.dy;
	}
}

void Screen::basicDraw() {
	// Lambda di supporto per non duplicare la logica di disegno
	auto drawEntitySprite = [&](entt::entity entity, transform& t, sprite& s, animation& a) {
		Rectangle source = {
			static_cast<float>(a.currentFrame * s.width),
			static_cast<float>(a.row * s.height),
			static_cast<float>(s.width),
			static_cast<float>(s.height)
		};

		float absScaleX = std::abs(t.scale.x);

		Rectangle dest = {
			t.position.x - s.width * t.scale.x / 2.0f,
			t.position.y - s.height * t.scale.y / 2.0f,
			s.width * absScaleX,
			s.height * t.scale.y
		};

		Vector2 origin = { 0.0f, 0.0f };

		if (t.scale.x < 0) {
			source.width = -source.width;
		}

		auto hf = registry.try_get<hit_flash>(entity);
		Color filter = hf ? hf->filter : WHITE;
		s.textures.at(s.currentTexture)->Draw(source, dest, origin, t.rotation, filter);
	};

	// 1. Inizia la modalità telecamera: spazio del mondo
	BeginMode2D(camera);

	// Disegna lo sfondo della mappa (se esiste) usando il tiling
	auto mapView = registry.view<is_map_background, transform, sprite>();
	for (auto entity : mapView) {
		auto& bg = mapView.get<is_map_background>(entity);
		auto& t = mapView.get<transform>(entity);
		auto& s = mapView.get<sprite>(entity);

		if (s.textures.count("default")) {
			auto& tileTexture = *s.textures.at("default");
			Rectangle source = { 0.0f, 0.0f, (float)tileTexture.width, (float)tileTexture.height };
			Rectangle dest = { t.position.x, t.position.y, t.scale.x, t.scale.y }; // Usa la scala del transform come dimensione totale
			Vector2 origin = { 0, 0 };
			if (bg.tiled == true) {
				DrawTextureTiled(tileTexture, source, dest, origin, 0.0f, 1.0f, WHITE);
			} else {
				DrawTexturePro(tileTexture, source, dest, origin, 0.0f, WHITE);
			}
		}
	}

	// Disegna solo le entità che NON fanno parte della UI (entt::exclude)
	auto worldView = registry.view<transform, sprite, animation>(entt::exclude<is_ui, is_map_background>);
	for (auto entity : worldView) {
		drawEntitySprite(entity, worldView.get<transform>(entity), worldView.get<sprite>(entity), worldView.get<animation>(entity));
	}

	EndMode2D();

	// 2. Disegna la UI (joystick, bottoni) fissa sullo schermo (senza telecamera)
	auto uiView = registry.view<transform, sprite, animation, is_ui>();
	for (auto entity : uiView) {
		drawEntitySprite(entity, uiView.get<transform>(entity), uiView.get<sprite>(entity), uiView.get<animation>(entity));
	}

	auto scriptView = registry.view<script>();
	for (auto entity : scriptView) {
		auto& s = scriptView.get<script>(entity);
		if (s.instance) {
			s.instance->onDraw();
		}
	}
}

void Screen::basicUnload() {
	registry.clear();
}

void Screen::updateScripts() {
	float dt = GetFrameTime();
	auto scriptView = registry.view<script>();

	for (auto entity : scriptView) {
		auto& s = scriptView.get<script>(entity);
		if (s.instance) {
			s.instance->onUpdate(dt);
		}
	}
}

void Screen::updateAnimations(float dt) {
	auto view = registry.view<animation, velocity>();
	for (auto entity : view) {
		auto& a = view.get<animation>(entity);
		auto& v = view.get<velocity>(entity);

		if (a.isPlaying) {
			if (std::abs(v.dx) > std::abs(v.dy)) {
				// Horizontal movement dominates
				a.row = (v.dx > 0) ? RIGHT : LEFT;  // right : left
			} else if (v.dy != 0) {
				// Vertical movement dominates
				a.row = (v.dy > 0) ? DOWN : UP;  // down : up
			}

			a.timer += dt;

			if (a.timer >= a.frameTime) {
				a.timer = 0.0f;
				a.currentFrame++;

				if (a.currentFrame > a.endFrame) {
					a.currentFrame = a.startFrame;
				}
			}
		}
	}
}