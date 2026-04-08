#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include <entt/entt.hpp>

#include <memory>

class AssetManager {
public:
	std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> textures;

	std::shared_ptr<raylib::Texture2D> loadTexture(const std::string& path);
	void unloadTexture(const std::string& path);
	void unloadAll();
};


