import socket

# create TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# bind to the port
ip = '127.0.0.1'
port = 7777
print('listening to {0} on port {1}'.format(ip, port))
sock.bind((ip,port))
sock.listen(1)

# spin on messages received
conn,addr = sock.accept()
print('opened connection with {0}'.format(addr))
while True:
    print('waiting for data...')
    data = conn.recv(16) # buf should be more like 1024
    print(type(data))

    print('received {0} bytes from {1}'.format(len(data), addr))
    print(data)

    if data is not None:
        reply = 'RECEIVED:{0}'.format(data)
        sent = conn.send(reply)
        print('sent {0} bytes back to {1}'.format(sent, addr))
conn.close()
sock.close()
