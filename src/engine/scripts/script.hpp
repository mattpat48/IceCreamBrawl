#pragma once

#include <entt/entt.hpp>
#include <memory>

class Script {
   public:
    virtual ~Script() = default;

    entt::entity entity;
    entt::registry* registry = nullptr;

    virtual void onCreate() {}
    virtual void onUpdate(float dt) {}
    virtual void onDraw() {}
    virtual void onDestroy() {}

    template <typename T>
    T* getComponent() {
        return registry->try_get<T>(entity);
    }
};