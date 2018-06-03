import socket

class BloopTCP:

    def __init__(self, local_ip)
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.local_ip = local_ip
        self.conn = None
        self.client_addr = None
        self.server_addr = None

    def listen_on(self, ip, port):
        self.is_server = True
        self.server_addr = (ip, port)
        self.sock.bind(self.server_addr)
        self.sock.listen(1)
        c,a = sock.accept()
        self.conn = c
        self.client_addr = a

    def connect_to(self, ip, port):
        self.is_client = True
        self.server_addr = (ip, port)
        self.sock.connect(self.server_addr)

    def disconnect():
        self.is_server = False
        self.is_client = False
        self.sock.close()
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
