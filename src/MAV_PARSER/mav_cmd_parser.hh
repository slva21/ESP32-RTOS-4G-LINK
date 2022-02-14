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
extern mavlink_command_long_t mav_cmd_long;

class MAV_PARSER
{

private:
    static const int FORCE_CMD = 21196;

public:
    static inline bool MAV_MSG_PARSER(const mavlink_message_t *msg, MAV_MSG_ENCODER *mav_msg_encoder)
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

        case MAVLINK_MSG_ID_POSITION_TARGET_LOCAL_NED: // Reports

            mavlink_msg_position_target_local_ned_decode(msg, &target_local_position);

            break;

        case MAVLINK_MSG_ID_POSITION_TARGET_GLOBAL_INT: // Reports

            mavlink_msg_position_target_global_int_decode(msg, &target_global_position);

            break;

            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SET COMMANDS ~~~~~~~~~~~~~~~~~~~~~~~~~~~//

        case MAVLINK_MSG_ID_COMMAND_LONG:

            mavlink_msg_command_long_decode(msg, &mav_cmd_long);

            mav_msg_encoder->encode_cmd_ack(MAV_PARSER::MAV_CMD_LONG_PARSER());

            return true; // Return true as ACK needs to be sent back

            break;

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

        return false;
    }

    static mavlink_command_ack_t MAV_CMD_LONG_PARSER()
    {
        mavlink_command_ack_t ACK;

        switch (mav_cmd_long.command)
        {
        case MAV_CMD_COMPONENT_ARM_DISARM:

            ACK.command = MAV_CMD_COMPONENT_ARM_DISARM;

            if (mav_cmd_long.param2 == FORCE_CMD)
            {
                PILOT_ARM_DISARM = mav_cmd_long.param1;

                ACK.result = 1;
            }
            else
            {
                // #TODO SAFTEY CHECKS
                PILOT_ARM_DISARM = mav_cmd_long.param1;
            }

            break;

        default:
            break;
        }

        return ACK;
    }
};
