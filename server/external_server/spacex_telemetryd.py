### TODO FIXME this is a hack to import bloop_comms from one directory up
import os.path
import sys
sys.path.append(os.path.abspath('..'))
###
import bloop_comms
import requests
import argparse
import time
import json

# constants for getting data from the DB
ROUTE_ALL = '/all'
SEND_MILLI_THRESH = 100 # 10Hz

def build_arg_parser():
    parser = argparse.ArgumentParser(description='Send the most recent telemetry to SpaceX at no greater than 10Hz.')
    parser.add_argument('telem_ip', help='the address of the telemetry server', nargs='?', default='127.0.0.1')
    parser.add_argument('telem_port', help='the port of the telemetry server', type=int, nargs='?', default=8080)
    parser.add_argument('spacex_ip', help='the address of the SpaceX endpoint', nargs='?', default='192.168.0.1')
    parser.add_argument('spacex_port', help='the port of the SpaceX endpoint', type=int, nargs='?', default=3000)
    return parser

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

def build_udp_endpoint(send_ip, send_port, recv_ip, recv_port):
    ep = bloop_comms.udp_endpoint(send_ip, send_port, recv_ip, recv_port)
    return ep

def get_curr_millis():
    # the time function returns seconds in floating point
    return int(time.time() * 1000)

def build_telem_url(ip, port):
    url = 'http://{0}:{1}{2}'.format(ip, port, ROUTE_ALL)
    return url

def get_all_telem(telem_url):
    # first hit the telemetry DB server
    r = requests.get(telem_url)
    data = tru_parse_json(r.text)
    return data

def build_spacex_packet(telem_url):
    # first try to grab all the telemetry
    telem_data = get_all_telem(telem_url)
    if telem_data is None:
        return None
    ########################################################
    # TODO actually build the proper packet with telemetry #
    ########################################################
    return 'dummy'

def send_loop(telem_url, udp_comm):
    # just keep sending
    last_send = 0
    while True:
        # has enough time passed to send another?
        curr_time = get_curr_millis()
        if (curr_time - last_send) < SEND_MILLI_THRESH:
            # not enough time has passed so wait and continue
            time.sleep(0.01)
            continue
        to_send = build_spacex_packet(telem_url)
        if to_send is not None:
            print('sending [{0}]'.format(to_send))
            udp_comm.send(to_send)
            last_send = get_curr_millis()

def main():
    # setup comms from the args
    args = build_arg_parser().parse_args()
    # build the url of the server for telemetry data
    telem_url = build_telem_url(args.telem_ip, args.telem_port)
    # and build a pipe to SpaceX (we don't really care about listening)
    udp_comm = build_udp_endpoint(args.spacex_ip, args.spacex_port, '127.0.0.1', 7000)
    # start the loop
    send_loop(telem_url, udp_comm)

if __name__ == '__main__':
    main()
