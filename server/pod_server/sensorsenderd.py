### TODO FIXME this is a hack to import bloop_comms from one directory up
import os.path
import sys
sys.path.append(os.path.abspath('..'))
###
import bloop_comms
import argparse
import random
import time
import json

# these should probably be added to a shared import
SENSOR_NAME = 'sensor_name'
SENSOR_VALUE = 'value'
TIMESTAMP = 'timestamp'

def build_arg_parser():
    parser = argparse.ArgumentParser(description='Read sensors on the pod and send them to the external server to store in the database.')
    parser.add_argument('ip', help='the address on which to listen')
    parser.add_argument('port', help='the port on which to listen', type=int)
    parser.add_argument('ext_ip', help='the address of the external DB server')
    parser.add_argument('ext_port', help='the port of the external DB server', type=int)
    return parser

def build_udp_endpoint(send_ip, send_port, recv_ip, recv_port):
    ep = bloop_comms.udp_endpoint(send_ip, send_port, recv_ip, recv_port)
    return ep

def read_sensor_data():
    # TODO collect sensor data and convert to a json string
    # TODO should all be coming as one big blob over SPI or I2C
    # get the current seconds since epoch (use datetime if string preferred)
    curr_time = int(time.time())
    # put together a dummy for testing
    ret = dict()
    ret[TIMESTAMP] = curr_time
    ret[SENSOR_NAME] = random.choice(['sensor_1', 'sensor_2', 'sensor_3'])
    ret[SENSOR_VALUE] = random.randint(0, 250)
    time.sleep(1.0)
    return json.dumps(ret)

def send_loop(udp_comm):
    # read and send forever
    while True:
        to_send = read_sensor_data()
        udp_comm.send(to_send)

def main():
    # setup comms from the args
    args = build_arg_parser().parse_args()
    udp_comm = build_udp_endpoint(args.ext_ip, args.ext_port, args.ip, args.port)
    # start reading and sending
    send_loop(udp_comm)

if __name__ == '__main__':
    main()
