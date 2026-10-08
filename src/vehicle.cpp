#include "vehicle.h"
#include "car_code/models.h"
#include "car_code/movement.h"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/vector3.hpp"

void Vehicle::_bind_methods()
{
    
}

void Vehicle::_process(double delta)
{
    if (Engine::get_singleton()->is_editor_hint())
        return;

    constexpr float SPEED = 1.0f;
    const auto& position = get_position();
    const auto& rotation = get_rotation();

    switch (get_movement_state()) 
    {
    case STOP:
        break;
    case TURN_LEFT:
        rotate_y(delta * SPEED);
        break;
    case TURN_RIGHT:
        rotate_y(-delta * SPEED);
        break;
    case MOVE_FORWARD:
    {
        auto global_trans = get_global_transform();
        godot::Vector3 forward_dir = -global_trans.basis.get_column(2);
        godot::Vector3 new_position = get_global_position() + forward_dir * SPEED * delta;
        set_global_position(new_position);
        break;
    }
    case MOVE_BACKWARD:
    {
        auto global_trans = get_global_transform();
        godot::Vector3 forward_dir = global_trans.basis.get_column(2);
        godot::Vector3 new_position = get_global_position() + forward_dir * SPEED * delta;
        set_global_position(new_position);
        break;
    }
    }
}
