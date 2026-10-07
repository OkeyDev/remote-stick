#include "network_class.h"

void NetworkClass::_bind_methods() {
	godot::ClassDB::bind_method(D_METHOD("init"), &NetworkClass::init);
	godot::ClassDB::bind_method(D_METHOD("update"), &NetworkClass::update);
}

int NetworkClass::init() {
	return net_init();
}
void NetworkClass::update() {
	net_update();
}
