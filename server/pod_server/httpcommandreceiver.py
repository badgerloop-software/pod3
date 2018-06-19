import argparse
import json

from bottle import route, run, request

# these should probably be added to a shared import (e.g. bloop_comms)
ROUTE_STATE_CHANGE = '/state_change'
ROUTE_POKE = '/poke'
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
    success = True
    message = None
    # TODO issue the new state change
    ##########################
    # STATE CHANGE CODE HERE #
    ##########################
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

# route to just see if the server is responding
@route(ROUTE_POKE, method=['GET', 'POST'])
def poke():
    return json_basic_response(success=True)

########
# MAIN #
########

def main():
    # start the webserver on the correct ip and port
    args = build_arg_parser().parse_args()
    run(host=args.ip, port=args.port)

if __name__ == '__main__':
    main()
