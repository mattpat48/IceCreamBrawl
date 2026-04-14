#pragma once

#include <entt/entt.hpp>
#include "engine/assetManager.hpp"
#include "defines/components/components.hpp"
#include "ui/buttons/pauseButton/pauseButton.hpp"
#include "raylib-cpp.hpp"

class PauseButtonFactory {
public:
    static entt::entity create(entt::registry& registry, AssetManager& assetManager, raylib::Vector2 position) {
        entt::entity buttonEntity = registry.create();

        registry.emplace<is_ui>(buttonEntity);

        // Assicurati che esista una texture per il pulsante di pausa in questo percorso!
        auto texture = assetManager.loadTexture("resources/sprites/buttons/pause.png");
        std::unordered_map<std::string, std::shared_ptr<raylib::Texture2D>> textures;
        textures["default"] = texture;

        int texWidth = texture ? texture->width : 32;
        int texHeight = texture ? texture->height : 32;

        registry.emplace<sprite>(buttonEntity, std::move(textures), "default", texWidth, texHeight);
        registry.emplace<transform>(buttonEntity, position, raylib::Vector2(4.0f, 4.0f), 0.0f);
        registry.emplace<animation>(buttonEntity, 0, 0, 0, 0, 0.1f, 0.0f, false, 0);

        registry.emplace<script>(buttonEntity).bind<PauseButton>(buttonEntity, registry);

        return buttonEntity;
    }
};