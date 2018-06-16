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
VALID_STATES = {'idle', 'pumpdown', 'ready', 'propulsion', 'braking', 'postrun', 'fault_prerun', 'fault'}

def build_arg_parser():
    parser = argparse.ArgumentParser(description='Send a state change request to the pod.')
    parser.add_argument('pod_ip', help='the address of the pod')
    parser.add_argument('pod_port', help='the port of the pod', type=int)
    parser.add_argument('state', help='the desired state to issue', choices=VALID_STATES)
    return parser

def build_tcp_sender(pod_ip, pod_port):
    sender = bloop_comms.tcp_sender(pod_ip, pod_port)
    return sender

def issue_command(tcp_comm, state):
    # build the dict
    cmd = dict()
    cmd[COMMAND] = CMND_STATE_CHANGE
    cmd[COMMAND_VALUE] = state
    # send it
    tcp_comm.send(json.dumps(cmd))

def main():
    # setup comms from the args
    args = build_arg_parser().parse_args()
    tcp_comm = build_tcp_sender(args.pod_ip, args.pod_port)
    issue_command(tcp_comm, args.state)

if __name__ == '__main__':
    main()
