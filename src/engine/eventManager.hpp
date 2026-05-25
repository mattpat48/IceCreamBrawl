#pragma once

#include <entt/entt.hpp>

class EventManager {
public:
    entt::dispatcher dispatcher;

    template<typename Event>
    void emit(const Event& e) {
        dispatcher.trigger<Event>(e);
    }

    template<typename Event, typename... Args>
    void emit(Args&&... args) {
        dispatcher.trigger<Event>(std::forward<Args>(args)...);
    }

    template<typename Event>
    auto& sink() {
        return dispatcher.sink<Event>();
    }
};
