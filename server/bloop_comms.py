import socket

class TCPSender:

    def __init__(self, ip, port):
        self.server_addr = (ip, port)
        self.sock = None

    def _try_connect(self, timeout):
        # create a new socket for this single message
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        if timeout is not None:
            self.sock.settimeout(timeout)
        connected = False
        try:
            self.sock.connect(self.server_addr)
            connected = True
        except:
            pass
        return connected

    def disconnect(self):
        if self.sock is not None:
            self.sock.close()
            self.sock = None

    def send(self, mesg, timeout=5.0):
        sent = -1
        success = self._try_connect(timeout)
        if success:
            sent = self.sock.send(mesg)
        self.disconnect()
        return sent

class TCPListener:

    def __init__(self):
        self.timeout = 1.0
        self.buf_size = 4096
        self.sock = None
        self.is_listening = False

    def _reconnect(self):
        self.disconnect()

    def _try_accept(self):
        conn = None
        addr = None
        try:
            conn,addr = self.sock.accept()
            conn.settimeout(self.timeout)
        except:
            # TODO do something smart depending on the exception
            conn = None
            addr = None
        return conn,addr

    def _try_recv(self, conn):
        # quick skip if need be
        if conn is None:
            return None
        # keep pulling the message until we have it all
        mesg = ''
        done = False
        while not done:
            data = ''
            try:
                data = conn.recv(self.buf_size)
            except:
                # TODO handle disconnect and reconnect if problems
                done = True
            mesg += data
            done = len(data) < self.buf_size
        return mesg

    def connect(self, ip, port):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_addr = (ip, port)
        # bind and listen
        self.sock.bind(self.server_addr)
        self.sock.listen(1)
        self.is_listening = True

    def disconnect(self):
        self.is_listening = False
        if self.sock is not None:
            self.sock.close()
            self.sock = None

    def recv(self):
        # we're just going to accept one message for each remote connection
        conn,addr = self._try_accept()
        # and try to pull something from the pipe
        mesg = self._try_recv(conn)
        # close that connection after we're done with it
        if conn is not None:
            # TODO deal with exceptions
            conn.close()
        return mesg,addr

class UDPEndpoint:

    def __init__(self, send_ip, send_port, recv_ip, recv_port):
        self.buf_size = 4096
        # just open two pipes for sending and receiving
        self.recv_addr = (recv_ip, recv_port)
        self.send_addr = (send_ip, send_port)
        self.recv_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.send_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.recv_sock.settimeout(1.0)
        self.send_sock.settimeout(1.0)
        self.recv_sock.bind(self.recv_addr)

    def disconnect(self):
        if self.recv_sock is not None:
            self.recv_sock.close()
            self.recv_sock = None
        if self.send_sock is not None:
            self.send_sock.close()
            self.send_sock = None

    def send(self, mesg):
        sent = -1
        try:
            sent = self.send_sock.sendto(mesg, self.send_addr)
        except:
            pass
        return sent

    def recv(self):
        # just make sure the buffer is big enough for any messages here
        mesg = None
        try:
            mesg = self.recv_sock.recv(self.buf_size)
        except:
            # TODO handle disconnect and reconnect if problems
            pass
        return mesg

####################################
### IGNORE THIS HOT MESS FOR NOW ###
####################################
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
        sent_bytes = 0
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

def tcp_listener(ip, port):
    l = TCPListener()
    l.connect(ip, port)
    return l

def tcp_sender(ip, port):
    s = TCPSender(ip, port)
    return s

def udp_endpoint(send_ip, send_port, recv_ip, recv_port):
    e = UDPEndpoint(send_ip, send_port, recv_ip, recv_port)
    return e

def _tcp_server(ip, port):
    b = BloopTCP()
    b._listen_on(ip, port)
    return b

def _tcp_client(ip, port):
    b = BloopTCP()
    b._connect_to(ip, port)
    return b
