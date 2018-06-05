import socket

class BloopTCP:

    def __init__(self):
        # do initial socket creation
        self.timeout = 1.0
        self.buf_size = 1024
        self.sock = None
        self._reinit()

    def _reinit(self):
        self.disconnect()
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.settimeout(self.timeout) # global timeouts for this guy
        self.is_server = False
        self.is_client = False
        self.server_addr = None
        self.client_addr = None
        self.conn = None

    def _spin_accept(self):
        connected = False
        conn = None
        addr = None
        # keep trying to accept until we have something
        while not connected:
            try:
                conn,addr = self.sock.accept()
                connected = True
            except:
                # maybe deal with this elegantly?
                pass
        return conn,addr

    def _spin_connect(self):
        connected = False
        # keep trying until we have a connection
        while not connected:
            try:
                self.sock.connect(self.server_addr)
                connected = True
            except:
                pass

    def _is_established(self):
        return self.is_server or self.is_client

    def _listen_on(self, ip, port):
        # reset if we're already initialized
        if self._is_established():
            self._reinit()
        # we're going to be the server
        self.is_server = True
        self.server_addr = (ip, port)
        # bind and listen on the ip and port (technically could error out)
        self.sock.bind(self.server_addr)
        self.sock.listen(1)
        # spin accept until a connection happens
        c,a = self._spin_accept()
        self.client_addr = a
        # c is a new socket connected to the client
        self.conn = c
        self.conn.settimeout(self.timeout)

    def _connect_to(self, ip, port):
        # reset if we're already initialized
        if self._is_established():
            self._reinit()
        self.is_client = True
        self.server_addr = (ip, port)
        self._spin_connect()
        # need to mock the sock as conn for clients (server has both)
        self.conn = self.sock

    def send(self, mesg):
        try:
            sent_bytes = self.conn.send(mesg)
        except:
            # TODO need to reconnect if disconnected
            pass
        return sent_bytes > 0

    def recv(self):
        mesg = ''
        done = False
        while not done:
            data = ''
            try:
                data = self.conn.recv(self.buf_size)
            except:
                # TODO reconnect if disconnected
                pass
            mesg += data
            done = len(data) < self.buf_size
        return mesg

    def disconnect(self):
        if self.sock is not None:
            self.sock.close()
            self.sock = None

def server(ip, port):
    b = BloopTCP()
    b._listen_on(ip, port)
    return b

def client(ip, port):
    b = BloopTCP()
    b._connect_to(ip, port)
    return b