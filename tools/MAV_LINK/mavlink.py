"""
Generate a message using different MAVLink versions, put in a buffer and then read from it.
"""

from serial import Serial
import socket
from time import sleep

from pymavlink import mavutil
from pymavlink.dialects.v20 import ardupilotmega as mavlink2

# open ardupilotmega line #16544


SER = Serial('COM4', 115200)


class fifo(object):
    def __init__(self):
        self.buf = []

    def write(self, data):
        self.buf += data
        return len(data)

    def read(self):
        return self.buf.pop(0)


def test_protocol(mavlink, signing=False):
    # we will use a fifo as an encode/decode buffer
    f = fifo()

    # print("Creating MAVLink message...")
    # create a mavlink instance, which will do IO on file object 'f'
    mav = mavlink.MAVLink(f)

    if signing:
        mav.signing.secret_key = bytearray(chr(42)*32, 'utf-8')
        mav.signing.link_id = 0
        mav.signing.timestamp = 0
        mav.signing.sign_outgoing = True

    # set the WP_RADIUS parameter on the MAV at the end of the link
    # mav.param_set_send(7, 9, b"WP_RADIUS", 101, mavlink.MAV_PARAM_TYPE_REAL32)

    # alternatively, produce a MAVLink_param_set object
    # this can be sent via your own transport if you like

    # m = mav.param_set_encode(7, 4, b"WP_RADIUS", 101,
    #                          mavlink.MAV_PARAM_TYPE_REAL32)

    # m.pack(mav)

    # glb = mav.wifi_config_ap_send('ssid','password',)

    glb = mav.global_position_int_encode(
        10, 57123456, -1412345, 10, -2, 1, 2, 3, 90)

    # glb = mav.set_position_target_local_ned_encode(100, 2, 1, mavutil.mavlink.MAV_FRAME_LOCAL_NED, int(
    #   0b110111000000), 10, 20, -30, 40, 50, 60, 70, 80, 90, 100, 110)

    # glb = mav.command_ack_encode(200, 1, 1, 0, 200, 1)

    # glb = mav.command_long_encode(
    #     1, 1, mavutil.mavlink.MAV_CMD_COMPONENT_ARM_DISARM, 0, 1, 0, 0, 0, 0, 0, 0)

    glb.pack(mav)

    # get the encoded message as a buffer
    msg_bytes = glb.get_msgbuf()

    SER.write(msg_bytes)

    while(SER.inWaiting() > 0):
        data = SER.read()
        try:
            msg = mav.parse_char(data)
            if(msg != None):
                print(msg.target_system)
        except:
            pass

    # decoding
    # while msg == None:  # Create timeout
    #     rcv = s.recv(1024)
    #     msg = mav.parse_char(rcv)

   # glb = mav.set_position_target_local_ned_encode(100, 2, 1, mavutil.mavlink.MAV_FRAME_LOCAL_NED, int(
     #   0b110111000000), 10, 20, -30, 40, 50, 60, 70, 80, 90, 100, 110)

    # glb.pack(mav)

    # get the encoded message as a buffer
    # msg_bytes = glb.get_msgbuf()

   # return msg_bytes

    # get the encoded message as a buffer
    # b = m.get_msgbuf()

    # return b

    # print(b)
    # SER.write(b)

    # bi = []
    # for c in b:
    #     bi.append(int(c))
    # #print("Buffer containing the encoded message:")
    # # print(bi)
    # #print("Decoding message...")
    # # decode an incoming message
    # m2 = mav.decode(b)
    # # show what fields it has
    # print("Got a message with id %u and fields %s" %
    #       (m2.get_msgId(), m2.get_fieldnames()))
    # # print out the fields
    # print(m2)
while 1:

    test_protocol(mavlink2)
    # SER.write(test_protocol(mavlink2))

   # sleep(0.1)

    # print(cmd)


# print("Testing mavlink1\n")
# test_protocol(mavlink1)

# with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
#     s.bind(('192.168.1.197', 65432))
#     s.listen(0)
#     conn, addr = s.accept()
#     with conn:
#         print('Connected by', addr)
#         while True:
#             conn.send(test_protocol(mavlink2))
#             data = conn.recv(1024)
#             print(data)


# print("\nTesting mavlink2 with signing\n")
# test_protocol(mavlink2, True)
