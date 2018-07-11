import argparse
import serial
import json

from multiprocessing import Lock

from bottle import route, run, request

# these should probably be added to a shared import (e.g. bloop_comms)
ROUTE_STATE_CHANGE = '/state_change'
ROUTE_PRIM_BRAKE_ONOFF = '/prim_brake_onoff'
ROUTE_PRIM_BRAKE_VENT = '/prim_brake_vent'
ROUTE_HIGH_VOLTAGE = '/high_voltage'
ROUTE_POD_HEARTBEAT = '/pod_heartbeat'
ROUTE_POKE = '/poke'
ROUTE_BLANK = '/'
COMMAND_VALUE = 'value'
UART_PORT = '/dev/ttyS0'
UART_BAUD = 115200
UART_CMD_START = '$|'
UART_CMD_STOP = '\n'
UART_STATE_CHANGE_TOK = 't'
UART_OVERRIDE_TOK = 'o'
UART_HEARTBEAT_TOK = 'h'

VALID_STATES = ['state_poweroff', 'state_idle', 'state_ready_for_pumpdown', 'state_pumpdown', 'state_ready', 'state_postrun', 'state_service_low_speed', 'state_safe_to_approach', 'state_prop_start_hyperloop', 'state_prop_start_openair', 'state_prop_start_extsub', 'state_prop_dsa_hyperloop', 'state_prop_dsa_openair', 'state_prop_dsa_extsub', 'state_braking_hyperloop', 'state_braking_openair', 'state_braking_extsub', 'state_fault_prerun', 'state_fault_run', 'state_fault_postrun'];

SERIAL_LOCK = Lock()
SERIAL_PORT = serial.Serial(UART_PORT, baudrate=UART_BAUD)

###########
# HELPERS #
###########

def build_arg_parser():
    parser = argparse.ArgumentParser(description='Receive commands (e.g. state machine changes) and execute them - HTTP version.')
    parser.add_argument('ip', help='the address on which to listen')
    parser.add_argument('port', help='the port on which to listen', type=int)
    return parser

def json_basic_response(success=False, message=None):
    ret = dict()
    if success is not None:
        ret['success'] = success
    if message is not None:
        ret['message'] = message
    return json.dumps(ret)

def write_to_uart(to_write):
    # make sure we only write one at a time
    SERIAL_LOCK.acquire()
    SERIAL_PORT.write(to_write)
    print('wrote to serial - ' + to_write)
    SERIAL_LOCK.release()

def package_command(command):
    return '{0}{1}{2}'.format(UART_CMD_START, command, UART_CMD_STOP)

####################
# COMMAND HANDLERS #
####################

def issue_state_change(new_state):
    print('requested state change: {0}'.format(new_state))
    success = True
    message = None
    # make the payload (strip the state_ from the beginning)
    new_state = new_state.replace('state_', '')
    payload = '{0}{1}'.format(UART_STATE_CHANGE_TOK, new_state)
    # and package it for uart
    to_send = package_command(payload)
    # and send
    write_to_uart(to_send)
    return success,message

def issue_prim_brake_onoff(onoff):
    print('requested primary brake: {0}'.format(onoff))
    success = True
    message = None
    # make the payload (attach the on or off)
    control_val = 'prim_brake_{0}'.format(onoff)
    payload = '{0}{1}'.format(UART_OVERRIDE_TOK, control_val)
    # and package it for uart
    to_send = package_command(payload)
    # and send
    write_to_uart(to_send)
    return success,message

def issue_prim_brake_vent(onoff):
    print('requested primary brake vent: {0}'.format(onoff))
    success = True
    message = None
    # make the payload (attach the on or off)
    control_val = 'prim_brake_vent_{0}'.format(onoff)
    payload = '{0}{1}'.format(UART_OVERRIDE_TOK, control_val)
    # and package it for uart
    to_send = package_command(payload)
    # and send
    write_to_uart(to_send)
    return success,message

def issue_high_voltage(enable_disable):
    print('requested high voltage: {0}'.format(enable_disable))
    success = True
    message = None
    # make the payload (attach the enable or disable)
    control_val = 'hv_{0}'.format(enable_disable)
    payload = '{0}{1}'.format(UART_OVERRIDE_TOK, control_val)
    # and package it for uart
    to_send = package_command(payload)
    # and send
    write_to_uart(to_send)
    return success,message

def issue_pod_heartbeat():
    print('sending a network heartbeat to the pod')
    success = True
    message = None
    # make the payload (only command type important but need payload)
    val = 'finn'
    payload = '{0}{1}'.format(UART_HEARTBEAT_TOK, val)
    # and package it for uart
    to_send = package_command(payload)
    # and send
    write_to_uart(to_send)
    return success,message

#####################
# ROUTE CONTROLLERS #
#####################

# route to request issue a state change
@route(ROUTE_STATE_CHANGE, method='POST')
def request_state_change():
    new_state = request.forms.get(COMMAND_VALUE)
    if new_state is not None:
        success,message = issue_state_change(new_state)
        return json_basic_response(success=success, message=message)
    return json_basic_response(success=False, message='no new state requested')

# route to command override the brakes
@route(ROUTE_PRIM_BRAKE_ONOFF, method='POST')
def request_prim_brake_onoff():
    onoff = request.forms.get(COMMAND_VALUE)
    if onoff is not None:
        success,message = issue_prim_brake_onoff(onoff)
        return json_basic_response(success=success, message=message)
    return json_basic_response(success=False, message='no on or off requested')

# route to command primary brake vent
@route(ROUTE_PRIM_BRAKE_VENT, method='POST')
def request_prim_brake_vent():
    onoff = request.forms.get(COMMAND_VALUE)
    if onoff is not None:
        success,message = issue_prim_brake_vent(onoff)
        return json_basic_response(success=success, message=message)
    return json_basic_response(success=False, message='no vent on or off requested')

# route to enable/disable high voltage
@route(ROUTE_HIGH_VOLTAGE, method='POST')
def request_high_voltage_enable_disable():
    endis = request.forms.get(COMMAND_VALUE)
    if endis is not None:
        success,message = issue_high_voltage(endis)
        return json_basic_response(success=success, message=message)
    return json_basic_response(success=False, message='no enable/disable requested')

# send a heartbeat to the nucleo
@route(ROUTE_POD_HEARTBEAT, method='POST')
def pod_heartbeat():
    success,message = issue_pod_heartbeat()
    return json_basic_response(success=success, message=message)

# route to just see if the server is responding
@route(ROUTE_POKE, method=['GET', 'POST'])
def poke():
    return json_basic_response(success=True)

@route(ROUTE_BLANK, method='GET')
def blank():
    return poke()

########
# MAIN #
########

def main():
    # start the webserver on the correct ip and port
    args = build_arg_parser().parse_args()
    run(host=args.ip, port=args.port)

if __name__ == '__main__':
    main()
