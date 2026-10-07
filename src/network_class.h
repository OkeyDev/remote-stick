#pragma once

#include "car_code/network.h"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"

using namespace godot;

class NetworkClass : public RefCounted {
	GDCLASS(NetworkClass, RefCounted)

protected:
	static void _bind_methods();

public:
	NetworkClass() = default;
	~NetworkClass() override = default;

	int init();
	void update();
};
