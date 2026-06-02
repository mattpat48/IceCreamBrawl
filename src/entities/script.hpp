#pragma once

#include "entt/entt.hpp"
#include "defines/events/gestureEvents.hpp"
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

    virtual void onTap(TapEvent& e) {}
    virtual void onSwipe(SwipeEvent& e) {}
    virtual void onLongTap(LongTapEvent& e) {}

    template <typename T>
    T* getComponent() {
        return registry->try_get<T>(entity);
    }
};