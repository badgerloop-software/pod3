### TODO FIXME this is a hack to import bloop_comms from one directory up
import os.path
import sys
sys.path.append(os.path.abspath('..'))
###
import bloop_comms
import argparse
import serial
import random
import time
import json

# these should probably be added to a shared import
SENSOR_NAME = 'sensor_name'
SENSOR_VALUE = 'value'
TIMESTAMP = 'timestamp'
UART_PORT = '/dev/ttyS0'
UART_BAUD = 115200
UART_CMD_START = '$|'

SERIAL_PORT = serial.Serial(UART_PORT, baudrate=UART_BAUD)

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

def read_until_newline():
    mesg = ''
    done = False
    while not done:
        c = SERIAL_PORT.read()
        mesg += c
        done = c == '\n'
    return mesg

def pull_command_from_message(mesg):
    # we assume this is a \n terminated string
    mesg = mesg.replace('\n', '')
    # then look for rightmost the start of a command
    cmd_start_ind = mesg.rfind(UART_CMD_START)
    if cmd_start_ind < 0:
        # no command start marker found
        return None
    # found a command so give it back
    return mesg[cmd_start_ind:]

def convert_command_to_json_payload(cmnd):
    # first remove the command indicator
    cmnd = cmnd.replace(UART_CMD_START, '')
    # it's just a comma separated pair of name and value
    parts = cmnd.split(',')
    sensor_name = parts[0]
    sensor_value = parts[1]
    # get the current seconds since epoch (use datetime if string preferred)
    curr_time = time.time()
    # and convert it from float seconds to integer milliseconds
    curr_time = int(curr_time * 1000)
    # and put those into a dict for json translation
    data = dict()
    data[TIMESTAMP] = curr_time
    data[SENSOR_NAME] = sensor_name
    data[SENSOR_VALUE] = sensor_value
    return json.dumps(data)

def read_sensor_data():
    # grab whatever we can off serial
    mesg = read_until_newline()
    # pull out the command data
    cmnd = pull_command_from_message(mesg)
    if cmnd is None:
        return None
    # and turn the command into a Mongo server message
    return convert_command_to_json_payload(cmnd)

def send_loop(udp_comm):
    # read and send forever
    while True:
        to_send = read_sensor_data()
        print to_send
        if to_send is not None:
            udp_comm.send(to_send)

def main():
    # setup comms from the args
    args = build_arg_parser().parse_args()
    udp_comm = build_udp_endpoint(args.ext_ip, args.ext_port, args.ip, args.port)
    # start reading and sending
    send_loop(udp_comm)

if __name__ == '__main__':
    main()
