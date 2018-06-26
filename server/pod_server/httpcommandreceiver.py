import argparse
import json

from bottle import route, run, request

# these should probably be added to a shared import (e.g. bloop_comms)
ROUTE_STATE_CHANGE = '/state_change'
ROUTE_PRIM_BRAKE_ONOFF = '/prim_brake_onoff'
ROUTE_PRIM_BRAKE_VENT = '/prim_brake_vent'
ROUTE_HIGH_VOLTAGE = '/high_voltage'
ROUTE_POKE = '/poke'
ROUTE_BLANK = '/'
COMMAND_VALUE = 'value'

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

####################
# COMMAND HANDLERS #
####################

def issue_state_change(new_state):
    print('requested state change: {0}'.format(new_state))
    success = True
    message = None
    # TODO issue the new state change
    ##########################
    # STATE CHANGE CODE HERE #
    ##########################
    return success,message

def issue_prim_brake_onoff(onoff):
    print('requested primary brake: {0}'.format(onoff))
    success = True
    message = None
    # TODO turn the brakes on or off
    return success,message

def issue_prim_brake_vent(onoff):
    print('requested primary brake vent: {0}'.format(onoff))
    success = True
    message = None
    # TODO issue it over SPI
    return success,message

def issue_high_voltage(enable_disable):
    print('requested high voltage: {0}'.format(enable_disable))
    success = True
    message = None
    # TODO issue it over SPI
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
