from encodings import utf_8
import random
import socket
import struct
from time import sleep


# value = -32.4444444
# ba = bytearray(struct.pack("f", value))
# print(ba)
# print(["0x%02x" % b for b in ba])


#print(0x254.to_bytes(8, 'big'))

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind(('192.168.1.197', 65432))
    s.listen(0)
    conn, addr = s.accept()
    with conn:
        print('Connected by', addr)
        while True:

            data = conn.recv(1024)
            print(data)

            # value = -32.899876
            # ba = bytearray(struct.pack("f", value))
            # conn.send(ba)
            # sleep(0.5)
