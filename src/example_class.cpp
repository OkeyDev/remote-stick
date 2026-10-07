#include "example_class.h"

void Example2Class::_bind_methods() {
	godot::ClassDB::bind_method(D_METHOD("print_type", "variant"), &Example2Class::print_type);
}

void Example2Class::print_type(const Variant &p_variant) const {
	print_line(vformat("Type: %d", p_variant.get_type()));
}
