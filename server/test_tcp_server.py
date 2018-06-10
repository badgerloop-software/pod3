import bloop_comms

ip = '127.0.0.1'
port = 7777

print('waiting for connection...')
listener = bloop_comms.tcp_listener(ip, port)

done = False
while not done:
    print('waiting for data...')
    mesg,addr = listener.recv()

    if mesg is not None and len(mesg) > 0:
        print('received [{0}] from {1}'.format(mesg, addr))
        if mesg in {'kill server'}:
            done = True

listener.disconnect()
