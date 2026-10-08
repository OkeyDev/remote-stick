#pragma once

#include "godot_cpp/classes/mesh_instance3d.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"

using namespace godot;

class Vehicle : public MeshInstance3D {
	GDCLASS(Vehicle, MeshInstance3D)
public:
	void _process(double delta) override;
protected:
	static void _bind_methods();
};
