import socket

# create UDP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# server address
ip = '127.0.0.1'
port = 7777
server_addr = (ip,port)
print('connecting to {0} on port {1}'.format(ip, port))

try:
    # get the user input
    mesg = raw_input('What would you like to send? ')
    print('sending {0}'.format(mesg))

    # send it
    sent = sock.sendto(mesg, server_addr)
    print('sent {0} bytes to {1}'.format(sent, server_addr))

    # receive response
    print('waiting for response...')
    data,addr = sock.recvfrom(16) # buf should be more like 4096
    print('received from {0}: {1}'.format(addr, data))
finally:
    print('closing socket')
    sock.close()
