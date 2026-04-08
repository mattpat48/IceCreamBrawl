#include "assetManager.hpp"

std::shared_ptr<raylib::Texture2D> AssetManager::loadTexture(const std::string& path) {
	auto it = textures.find(path);
	if (it != textures.end()) {
		return it->second;
	} else {
		auto texture = std::make_shared<raylib::Texture2D>(path);
		textures[path] = texture;
		return texture;
	}
}

void AssetManager::unloadTexture(const std::string& path) {
	auto it = textures.find(path);
	if (it != textures.end()) {
		it->second->Unload();
		textures.erase(it);
	}
}

void AssetManager::unloadAll() {
	for (auto& pair : textures) {
		pair.second->Unload();
	}
	textures.clear();
}