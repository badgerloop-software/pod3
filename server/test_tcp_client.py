import bloop_comms

ip = '127.0.0.1'
port = 7777
print('connecting to {0} on port {1}'.format(ip, port))
client = bloop_comms.tcp_client(ip, port)

done = False
while not done:
    try:
        # get the user input
        mesg = raw_input('What would you like to send? ')

        # check if we want to quit
        done = mesg in {'quit', 'exit'}

        # send it
        sent = client.send(mesg)
        print('sent {0} bytes'.format(sent))
    except:
        pass

client.disconnect()
