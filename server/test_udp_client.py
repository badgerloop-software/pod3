import bloop_comms

ip = '127.0.0.1'
recv_port = 7778
send_port = 7777

udp_ep = bloop_comms.udp_endpoint(ip, send_port, ip, recv_port)

done = False
while not done:
    # get the user input
    mesg = raw_input('What would you like to send? ')

    # send it
    sent = udp_ep.send(mesg)
    print('sent {0} bytes'.format(sent))

    # quit if needed
    if mesg in {'exit', 'quit'}:
        done = True

udp_ep.disconnect()
