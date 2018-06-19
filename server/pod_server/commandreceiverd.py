### TODO FIXME this is a hack to import bloop_comms from one directory up
import os.path
import sys
sys.path.append(os.path.abspath('..'))
###
import bloop_comms
import argparse
import json

# these should probably be added to a shared import (e.g. bloop_comms)
COMMAND = 'command'
COMMAND_VALUE = 'value'
CMND_STATE_CHANGE = 'state_change'

def build_arg_parser():
    parser = argparse.ArgumentParser(description='Receive commands (e.g. state machine changes) and execute them.')
    parser.add_argument('ip', help='the address on which to listen')
    parser.add_argument('port', help='the port on which to listen', type=int)
    return parser

def build_tcp_listener(recv_ip, recv_port):
    listener = bloop_comms.tcp_listener(recv_ip, recv_port)
    return listener

def try_parse_json(mesg):
    # shortcut if nothing to do
    if mesg is None:
        return None
    # otherwise try your best
    parsed = None
    try:
        parsed = json.loads(mesg)
    except:
        pass
    return parsed

def receive_command(tcp_comm):
    mesg,addr = tcp_comm.recv()
    return try_parse_json(mesg)

def execute_state_change(command):
    if command is None:
        return
    # get the desired state value
    new_state = command.get(COMMAND_VALUE)
    # TODO issue the new state change
    ##########################
    # STATE CHANGE CODE HERE #
    ##########################

def handle_command(command):
    if command is None:
        return
    # try to get the command type
    cmnd = command.get(COMMAND)
    # and handle it according to type
    if cmnd is None:
        return
    elif cmnd == CMND_STATE_CHANGE:
        execute_state_change(command)
    else:
        # got something funky
        return

def listen_loop(tcp_comm):
    # receive and execute forever
    while True:
        command = receive_command(tcp_comm)
        handle_command(command)

def main():
    # setup comms from the args
    args = build_arg_parser().parse_args()
    tcp_comm = build_tcp_listener(args.ip, args.port)
    # listening and executing
    listen_loop(tcp_comm)

if __name__ == '__main__':
    main()
