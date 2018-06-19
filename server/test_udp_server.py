import bloop_comms

ip = '127.0.0.1'
recv_port = 7777
send_port = 7778

udp_ep = bloop_comms.udp_endpoint(ip, send_port, ip, recv_port)

# spin on messages received
done = False
while not done:
    print('waiting for data...')
    mesg = udp_ep.recv()

    if mesg is not None and len(mesg) > 0:
        print('received [{0}]'.format(mesg))
        if mesg in {'kill server'}:
            done = True

udp_ep.disconnect()
