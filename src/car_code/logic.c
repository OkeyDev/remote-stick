#include "logic.h"

void init() {
	net_init();
}

void update() {
	net_update();
	if (net_connection_active()) {
		MovementState state = net_get_current_status();
		movement_set_state(state);
	} else {
		movement_set_state(STOP);
	}
}

void stop() {
	net_stop();
}
