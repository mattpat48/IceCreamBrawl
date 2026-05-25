#include "entt/entt.hpp"

template<typename Event>
void emit(const Event& e) {
	dispatcher.trigger<Event>(e);
}
