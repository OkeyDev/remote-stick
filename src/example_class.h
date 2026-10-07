#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"

using namespace godot;

class Example2Class : public RefCounted {
	GDCLASS(Example2Class, RefCounted)

protected:
	static void _bind_methods();

public:
	Example2Class() = default;
	~Example2Class() override = default;

	void print_type(const Variant &p_variant) const;
};
