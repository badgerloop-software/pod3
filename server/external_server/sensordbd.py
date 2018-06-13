### TODO FIXME this is a hack to import bloop_comms from one directory up
import os.path
import sys
sys.path.append(os.path.abspath('..'))
###
import bloop_comms
import argparse
import pymongo
import json

DB_NAME = 'pod_data'
SENSOR_ID = 'sensor_id'

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
    # grab the appropriate collection by the sensor id
    sensor_id = data.get(SENSOR_ID)
    if sensor_id is not None:
        coll = db[sensor_id]
        # TODO insert the appropriate data into the collection
        pass

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