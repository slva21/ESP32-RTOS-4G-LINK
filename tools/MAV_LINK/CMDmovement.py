from numpy import true_divide
from pymavlink import mavutil

from time import sleep

import socket
from serial import Serial


# the_connection = mavutil.mavlink_connection(
#     'tcp:sorair.mywire.org:6544')

the_connection = mavutil.mavlink_connection(
    'com4')


print("here")


#the_connection = mavutil.mavlink_connection('com4')
print(the_connection.wait_heartbeat())

while 1:

    the_connection.mav.command_long_send(the_connection.target_system, the_connection.target_component,
                                         mavutil.mavlink.MAV_CMD_COMPONENT_ARM_DISARM, 0, 1, 0, 0, 0, 0, 0, 0)

    msg = the_connection.recv_match(type='COMMAND_ACK', blocking=True)
    print(msg)

    sleep(1)


# while (1):

#     # 84
#     cmd = mavutil.mavlink.MAVLink_set_position_target_local_ned_message(100, the_connection.target_system, the_connection.target_comp  onent, mavutil.mavlink.MAV_FRAME_LOCAL_NED, int(0b110111000000), 10, 20, -30, 40, 50, 60, 70, 80, 90, 100, 110)

#     # print(cmd.__dict__)

#     the_connection.mav.send(cmd)

# print(cmd)

# if(SER.inWaiting() > 0):
#     data = SER.readline().decode('utf-8')
#     print(data)


# with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
#     s.bind(('172.20.10.9', 65432))
#     s.listen(0)
#     conn, addr = s.accept()
#     with conn:
#         print('Connected by', addr)
#         while True:
#             conn.send(mavutil.mavlink.MAVLink_set_position_target_local_ned_message(
#                 10, 1, 3, mavutil.mavlink.MAV_FRAME_LOCAL_NED, int(0b110111000000), 10, 0, -10, 0, 0, 0, 0, 0, 0, 0, 0).native_format)
#             data = conn.recv(1024)
#             print(data)
#             sleep(0.1)
