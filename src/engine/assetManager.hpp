#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include <entt/entt.hpp>

#include <memory>

class AssetManager {
public:
	std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> textures;
	std::unordered_map<std::string, std::shared_ptr<raylib::Sound>> sounds;

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

	std::shared_ptr<raylib::Sound> loadSound(const std::string& path) {
		auto it = sounds.find(path);
		if (it != sounds.end()) {
			return it->second;
		} else {
			auto sound = std::make_shared<raylib::Sound>(path);
			sounds[path] = sound;
			return sound;
		}
	}

	void unloadTexture(const std::string& path) {
		auto it = textures.find(path);
		if (it != textures.end()) {
			it->second->Unload();
			textures.erase(it);
		}
	}

	void unloadTextures() {
		for (auto& pair : textures) {
			pair.second->Unload();
		}
		textures.clear();
	}

	void unloadSound(const std::string& path) {
		auto it = sounds.find(path);
		if (it != sounds.end()) {
			it->second->Unload();
			sounds.erase(it);
		}
	}

	void unloadSounds() {
		for (auto& pair : sounds) {
			pair.second->Unload();
		}
		sounds.clear();
	}

	void unloadAll() {
		unloadTextures();
		unloadSounds();
	}
};


