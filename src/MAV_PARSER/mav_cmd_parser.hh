#pragma once
#include "../lib\mavlink\common\mavlink.h"
#include "mav_msg_encoder.hh"

extern mavlink_global_position_int_t global_position;
extern mavlink_param_set_t param_set;
extern mavlink_set_position_target_local_ned_t set_target_local_ned;
extern mavlink_set_position_target_global_int_t set_target_global_ned;
extern mavlink_set_attitude_target_t set_target_attitude;
extern mavlink_set_mode_t set_mode;
extern mavlink_set_home_position_t set_position_home;

class MAV_CMD_PARSER
{
private:
public:
    static inline bool MAV_PARSER(const mavlink_message_t *msg, MAV_MSG_ENCODER *mav_msg_encoder)
    {
        switch (msg->msgid)
        {

        case MAVLINK_MSG_ID_HEARTBEAT:

            mav_msg_encoder->encode_heartbeat();
            break;

        case MAVLINK_MSG_ID_SET_MODE:

            mavlink_msg_set_mode_decode(msg, &set_mode);

            break;

        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
            mavlink_msg_global_position_int_decode(msg, &global_position);

            Serial.printf("long: %d, lat: %d, alt: %d, rel_alt: %d, vx: %d, vy: %d, vz: %d, hdg: %d \n", global_position.lon, global_position.lat, global_position.alt, global_position.relative_alt, global_position.vx, global_position.vy, global_position.vz, global_position.hdg);

        case MAVLINK_MSG_ID_POSITION_TARGET_LOCAL_NED: // Reports

            mavlink_msg_position_target_local_ned_decode(msg, &target_local_position);

            break;

        case MAVLINK_MSG_ID_POSITION_TARGET_GLOBAL_INT: // Reports

            mavlink_msg_position_target_global_int_decode(msg, &target_global_position);

            break;

            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SET COMMANDS ~~~~~~~~~~~~~~~~~~~~~~~~~~~//

            // ~~~~~~~~~~~~~~~~~ MOVEMENT ~~~~~~~~~~~~~~~~~~//
        case MAVLINK_MSG_ID_SET_POSITION_TARGET_LOCAL_NED:

            mavlink_msg_set_position_target_local_ned_decode(msg, &set_target_local_ned);

            break;

        case MAVLINK_MSG_ID_SET_POSITION_TARGET_GLOBAL_INT:

            mavlink_msg_set_position_target_global_int_decode(msg, &set_target_global_ned);

            break;

        case MAVLINK_MSG_ID_SET_ATTITUDE_TARGET: // Suported in NO GPS Mode ONLY!

            mavlink_msg_set_attitude_target_decode(msg, &set_target_attitude);

            break;

            // ~~~~~~~~~~~~~~~~~ NAV SETTINGS ~~~~~~~~~~~~~~~~~~//
        case MAVLINK_MSG_ID_SET_HOME_POSITION:

            mavlink_msg_set_home_position_decode(msg, &set_position_home);

            break;

        default:
            break;
        }

        return true;
    }
};
