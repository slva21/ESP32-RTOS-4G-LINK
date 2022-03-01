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

extern mavlink_system_t mavlink_system;

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

            mavlink_msg_command_long_decode(msg, &mav_cmd_long); // Decode the msg object(mavlink message) and populate mav_cmd_long object

            mav_msg_encoder->encode_cmd_ack(MAV_PARSER::MAV_CMD_LONG_PARSER()); // Parse the mav_cmd_long object and return an ack_msg object
                                                                                // Encode the the ack_msg object into a mavlink msg obj and stores it in the *mav_msg_encoder object

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
        ACK.target_component = mavlink_system.compid;
        ACK.target_system = mavlink_system.sysid;

        switch (mav_cmd_long.command)
        {
        case MAV_CMD_COMPONENT_ARM_DISARM:

            ACK.command = MAV_CMD_COMPONENT_ARM_DISARM;

            if (mav_cmd_long.param2 == FORCE_CMD)
            {
                PILOT_ARM_DISARM = mav_cmd_long.param1;

                ACK.result = MAV_RESULT_ACCEPTED;
            }
            else
            {
                // #TODO SAFTEY CHECKS
                PILOT_ARM_DISARM = mav_cmd_long.param1;
            }

            break;

        case MAV_CMD_SET_MESSAGE_INTERVAL:

            ACK.command = MAV_CMD_SET_MESSAGE_INTERVAL;
            break;

        case MAV_CMD_DO_SET_MODE:

            ACK.command = MAV_CMD_DO_SET_MODE;
            break;

        case MAV_CMD_CONDITION_YAW: // Copter only
            ACK.command = MAV_CMD_CONDITION_YAW;
            break;

        case MAV_CMD_DO_FLIGHTTERMINATION: // disarms motors immediately (Copter falls!).

            ACK.command = MAV_CMD_CONDITION_YAW;

            if (mav_cmd_long.param1 > 0.5)
            {
                ACK.result = MAV_RESULT_ACCEPTED;
                // Terminate Flight
            }
            else
            {
                ACK.result = MAV_RESULT_FAILED; // or MAV_RESULT_UNSUPPORTED if insupported
            }

            break;

        case MAV_CMD_DO_PARACHUTE:

            ACK.command = MAV_CMD_DO_PARACHUTE;

            if (mav_cmd_long.param1 == 2)
                ACK.result = MAV_RESULT_ACCEPTED; // Release parachute

            else if (mav_cmd_long.param1 == 1)
                ACK.result = MAV_RESULT_ACCEPTED; // Endable Parachute

            else if (mav_cmd_long.param1 == 0)
                ACK.result = MAV_RESULT_ACCEPTED; // Disable

            break;

        default:
            break;
        }

        return ACK;
    }
};
