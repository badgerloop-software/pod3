import bloop_comms

ip = '127.0.0.1'
port = 7777

print('waiting for connection...')
server = bloop_comms.server(ip, port)

while True:
    print('waiting for data...')
    mesg = server.recv()

    if len(mesg) > 0:
        print('received [{0}] from {1}'.format(mesg, server.client_addr))

conn.close()
sock.close()
