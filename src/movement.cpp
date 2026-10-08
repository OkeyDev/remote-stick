#include "car_code/movement.h"
#include "car_code/models.h"

static MovementState s_state = MovementState::STOP;

void movement_set_state(MovementState state)
{
    s_state = state;
}

MovementState get_movement_state()
{
    return s_state;
}