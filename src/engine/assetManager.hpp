#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include <entt/entt.hpp>

#include <memory>

class AssetManager {
public:
	std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> textures;

	std::shared_ptr<raylib::Texture2D> loadTexture(const std::string& path) {
		auto it = textures.find(path);
		if (it != textures.end()) {
			return it->second;
		} else {
			auto texture = std::make_shared<raylib::Texture2D>(path);
			textures[path] = texture;
			return texture;
		}
	}

	void unloadTexture(const std::string& path) {
		auto it = textures.find(path);
		if (it != textures.end()) {
			it->second->Unload();
			textures.erase(it);
		}
	}

	void unloadAll() {
		for (auto& pair : textures) {
			pair.second->Unload();
		}
		textures.clear();
	}
};


