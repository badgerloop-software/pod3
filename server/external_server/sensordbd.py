### TODO FIXME this is a hack to import bloop_comms from one directory up
import os.path
import sys
sys.path.append(os.path.abspath('..'))
###
import bloop_comms
import argparse
import pymongo
import json

# these should probably be added to a shared import
DB_NAME = 'pod_data'
SENSOR_NAME = 'sensor_name'
SENSOR_DATA_ARR = 'sensor_data'
SENSOR_VALUE = 'value'
TIMESTAMP = 'timestamp'

def build_arg_parser():
    parser = argparse.ArgumentParser(description='Accept incoming sensor readings from the pod and store them in the database.')
    parser.add_argument('ip', help='the address on which to listen')
    parser.add_argument('port', help='the port on which to listen', type=int)
    parser.add_argument('pod_ip', help='the address of the pod')
    parser.add_argument('pod_port', help='the port of the pod', type=int)
    return parser

def build_udp_endpoint(ip, port, pod_ip, pod_port):
    ep = bloop_comms.udp_endpoint(pod_ip, pod_port, ip, port)
    return ep

def open_db():
    client = pymongo.MongoClient()
    db = client[DB_NAME]
    return db

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

def handle_database_entry(data, db):
    print(data)
    # grab the appropriate collection by the sensor name
    sensor_name = data.get(SENSOR_NAME)
    if sensor_name is not None:
        coll = db[sensor_name]
        # each collection just has one document labeled with the sensor name
        doc_match = {SENSOR_NAME: sensor_name}
        # push the latest datapoint onto the array
        sensor_val = data.get(SENSOR_VALUE)
        timestamp = data.get(TIMESTAMP)
        payload = {TIMESTAMP: timestamp, SENSOR_VALUE: sensor_val}
        arr_push = {'$push': {SENSOR_DATA_ARR: payload}}
        coll.update_one(doc_match, arr_push)

def handle_incoming(udp_comm, db):
    # see if we got anything
    mesg = udp_comm.recv()
    data = try_parse_json(mesg)
    # if so, try to put it into the database
    if data is not None:
        handle_database_entry(data, db)

def listen_loop(udp_comm, db):
    # listen forever
    while True:
        handle_incoming(udp_comm, db)

def main():
    # setup comms from the args
    args = build_arg_parser().parse_args()
    udp_comm = build_udp_endpoint(args.ip, args.port, args.pod_ip, args.pod_port)
    # and get a connection to our DB
    db = open_db()
    # start listening
    listen_loop(udp_comm, db)

if __name__ == '__main__':
    main()
