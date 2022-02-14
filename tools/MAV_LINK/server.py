import socket
import threading

from numpy import array

#6543 - 6550


def handle_conn0(conn, addr, filenum):
    with conn[0] and conn[1]:
        conn[1].send(conn[0].recv(1024))


def handle_conn1(conn, addr, filenum):
    with conn[1] and conn[0]:
        conn[0].send(conn[1].recv(1024))


def handle_conn(con, addr, filenum):
    with con:
        print(con.recv(1024))


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind(('', 65432))
    s.listen(0)
    num = 0
    conn = []
    addr = []
    while 1:
        con, adr = s.accept()
        conn.append(con)
        addr.append(adr)
        print('Connected by', addr[num])

        num += 1
        if(len(conn) == 2):
            threading.Thread(target=handle_conn0,
                             args=(conn, addr, num,)).start()
            threading.Thread(target=handle_conn1,
                             args=(conn, addr, num,)).start()
            conn.pop(0)
            conn.pop(0)
        # else:
        #     threading.Thread(target=handle_conn,
        #                      args=(conn[0], addr, num,)).start()
