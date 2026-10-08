#include "network_class.h"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/thread.hpp"
#include "godot_cpp/variant/callable_method_pointer.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

void NetworkClass::_bind_methods() {
}

void NetworkClass::_ready() {
	if (Engine::get_singleton()->is_editor_hint())
		return;

	net_init();

	m_thread.instantiate();
	m_mutex.instantiate();
	UtilityFunctions::print("Starting net_update thread");
	
	m_thread->start(callable_mp_static(net_update), Thread::PRIORITY_HIGH);
}

void NetworkClass::_exit_tree()
{
	if (m_thread.is_valid())
		m_thread->wait_to_finish();
	
	m_thread.unref();
}
