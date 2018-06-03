import socket

# create UDP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# bind to the port
ip = '127.0.0.1'
port = 7777
print('listening to {0} on port {1}'.format(ip, port))
sock.bind((ip,port))

# spin on messages received
while True:
    print('waiting for data...')
    data,addr = sock.recvfrom(16) # buf should be more like 4096

    print('received {0} bytes from {1}'.format(len(data), addr))
    print(data)

    if data is not None:
        reply = 'RECEIVED:{0}'.format(data)
        sent = sock.sendto(reply, addr)
        print('sent {0} bytes back to {1}'.format(sent, addr))
