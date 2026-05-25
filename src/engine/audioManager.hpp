#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"

#include <string>
#include <unordered_map>

class AudioManager {
public:
    std::unordered_map<std::string, Sound> sounds;
    std::unordered_map<std::string, Music> musics;

    Sound& loadSound(const std::string& path) {
        auto it = sounds.find(path);
        if (it != sounds.end()) return it->second;
        Sound s = LoadSound(path.c_str());
        sounds.emplace(path, s);
        return sounds[path];
    }

    void playSound(const std::string& path, float volume = 1.0f) {
        auto it = sounds.find(path);
        if (it == sounds.end()) {
            loadSound(path);
            it = sounds.find(path);
        }
        SetSoundVolume(it->second, volume);
        PlaySound(it->second);
    }

    Music& loadMusic(const std::string& path) {
        auto it = musics.find(path);
        if (it != musics.end()) return it->second;
        Music m = LoadMusicStream(path.c_str());
        musics.emplace(path, m);
        return musics[path];
    }

    void playMusic(const std::string& path, float volume = 1.0f, bool loop = true) {
        auto it = musics.find(path);
        if (it == musics.end()) {
            loadMusic(path);
            it = musics.find(path);
        }
        SetMusicVolume(it->second, volume);
        it->second.looping = loop;
        PlayMusicStream(it->second);
    }

    void updateMusicStreams() {
        for (auto& p : musics) {
            UpdateMusicStream(p.second);
        }
    }

    void unloadSound(const std::string& path) {
        auto it = sounds.find(path);
        if (it != sounds.end()) {
            UnloadSound(it->second);
            sounds.erase(it);
        }
    }

    void unloadMusic(const std::string& path) {
        auto it = musics.find(path);
        if (it != musics.end()) {
            UnloadMusicStream(it->second);
            musics.erase(it);
        }
    }

    void unloadAll() {
        for (auto& p : sounds) UnloadSound(p.second);
        for (auto& p : musics) UnloadMusicStream(p.second);
        sounds.clear();
        musics.clear();
    }
};
