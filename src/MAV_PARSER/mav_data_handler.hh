#pragma once
#include "../lib\mavlink\common\mavlink.h"

// File holds the structs that contain the current state definitions in the autopilot

uint8_t PILOT_STATE = 0;

mavlink_global_position_int_t global_position;
mavlink_param_set_t param_set;
mavlink_position_target_local_ned_t target_local_position;
mavlink_position_target_global_int_t target_global_position;

//__________________________________________ Set commands___________________________

// <~~~~~~~~~~~~~~~~~~~~~~~~ Movement commands
mavlink_set_position_target_local_ned_t set_target_local_ned;
mavlink_set_position_target_global_int_t set_target_global_ned;
mavlink_set_attitude_target_t set_target_attitude; // Supported in non GPS mode

// <~~~~~~~~~~~~~~~~~~~~~~~Nav Settings
mavlink_set_home_position_t set_position_home;

// <~~~~~~~~~~~~~~~~~~~~~~~~ Vihicle Settings
mavlink_set_mode_t set_mode;

