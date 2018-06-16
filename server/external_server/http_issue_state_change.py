import requests
import argparse

# these should probably be added to a shared import (e.g. bloop_comms)
ROUTE_STATE_CHANGE = '/state_change'
COMMAND_VALUE = 'value'
VALID_STATES = {'idle', 'pumpdown', 'ready', 'propulsion', 'braking', 'postrun', 'fault_prerun', 'fault'}

def build_arg_parser():
    parser = argparse.ArgumentParser(description='Send a state change request to the pod - HTTP version.')
    parser.add_argument('pod_ip', help='the address of the pod')
    parser.add_argument('pod_port', help='the port of the pod', type=int)
    parser.add_argument('state', help='the desired state to issue', choices=VALID_STATES)
    return parser

def build_url(pod_ip, pod_port):
    url = 'http://{0}:{1}{2}'.format(pod_ip, pod_port, ROUTE_STATE_CHANGE)
    return url

def build_payload(new_state):
    return {COMMAND_VALUE: new_state}

def main():
    # setup comms from the args
    args = build_arg_parser().parse_args()
    url = build_url(args.pod_ip, args.pod_port)
    payload = build_payload(args.state)
    r = requests.post(url, data=payload)
    # TODO something a little nicer on the response feedback
    print(r.text)

if __name__ == '__main__':
    main()
