#pragma once
#include "../lib\mavlink\common\mavlink.h"

mavlink_system_t mavlink_system = {
    1, // System ID (1-255)
    1  // Component ID (a MAV_COMPONENT value)
};

extern mavlink_set_mode_t set_mode;
extern uint8_t PILOT_STATE;

class MAV_MSG_ENCODER
{
public:
    uint8_t rtn_buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t rtn_buffer_len;

public:
    inline void encode_heartbeat()
    {
        // Define the system type (see mavlink_types.h for list of possible types)
        int system_type = 1;
        uint8_t base_mode = set_mode.base_mode;
        uint32_t custom_mode = set_mode.custom_mode;
        uint8_t system_status = PILOT_STATE;
        int autopilot_type = MAV_AUTOPILOT_GENERIC;

        // Initialize the required buffers
        mavlink_message_t msg; // Create message container

        // Pack the message
        mavlink_msg_heartbeat_pack(mavlink_system.sysid, mavlink_system.compid, &msg, system_type, autopilot_type, base_mode, custom_mode, system_status);

        // Copy the message to send buffer
        rtn_buffer_len = mavlink_msg_to_send_buffer(rtn_buffer, &msg);
    }

    // Encode a command ACK message
    inline void encode_cmd_ack(mavlink_command_ack_t command_ack)
    {
        // Initialize the required buffers
        mavlink_message_t msg; // Create message container

        mavlink_msg_command_ack_pack(mavlink_system.sysid, mavlink_system.compid, &msg, command_ack.command, command_ack.result, command_ack.progress, command_ack.result_param2, command_ack.target_system, command_ack.target_component);

        rtn_buffer_len = mavlink_msg_to_send_buffer(rtn_buffer, &msg);
    }
};
