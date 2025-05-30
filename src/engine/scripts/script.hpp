#pragma once

#include <entt/entt.hpp>
#include <memory>

class Script {
   public:
    virtual ~Script() = default;

    entt::entity entity;
    entt::registry* registry = nullptr;

    virtual void onCreate() {}
    virtual void onDestroy() {}
    virtual void onUpdate(float dt) {}
    virtual void onDraw() {}

    template <typename T>
    T* getComponent() {
        return registry->try_get<T>(entity);
    }
};