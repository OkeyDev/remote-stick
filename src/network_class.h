#pragma once

#include "car_code/network.h"
#include "godot_cpp/classes/mutex.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/thread.hpp"
#include "godot_cpp/classes/wrapped.hpp"

using namespace godot;

class NetworkClass : public Node {
	GDCLASS(NetworkClass, Node)

protected:
	static void _bind_methods();

public:
	NetworkClass() = default;
	~NetworkClass() override = default;

	void _ready() override;
	void _exit_tree() override;
private:
	Ref<Thread> m_thread;
	Ref<Mutex> m_mutex;
};
