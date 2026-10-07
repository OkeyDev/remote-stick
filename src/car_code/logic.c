#include "logic.h"

void init() {
	net_init();
}

void update() {
	net_update();
	if (net_connection_active()) {
		MovementState state = net_get_current_status();
		movemnt_set_state(state);
	} else {
		movemnt_set_state(STOP);
	}
}

void stop() {
	net_stop();
}
