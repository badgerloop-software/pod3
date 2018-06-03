import socket

# create TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# server address
ip = '127.0.0.1'
port = 7777
server_addr = (ip,port)
print('connecting to {0} on port {1}'.format(ip, port))

try:
    # get the user input
    mesg = raw_input('What would you like to send? ')
    print('sending {0}'.format(mesg))

    # connect
    sock.connect((ip,port))

    # send it
    sent = sock.send(mesg)
    print('sent {0} bytes to {1}'.format(sent, server_addr))

    # receive response
    print('waiting for response...')
    data = sock.recv(16) # buf should be more like 4096
    print('received {0}'.format(data))
finally:
    print('closing socket')
    sock.close()
