
"""
Badgerloop - Serial Port Interaction
"""

# built-in
import subprocess
import os

def open_connection(port, baud=115200):
    serial_args = str(baud) + ",cs8,-parenb,-cstopb,-hupcl"
    subprocess.call(
        ["screen", port, serial_args], stdout=open(os.devnull, 'wb')
    )

def get_available_ports():
    """ find all /dev/ttyACM* and /dev/ttyUSB* """

    ret = []
    devices = subprocess.check_output(["ls", "/dev"])
    dev_list = devices.split()
    for dev in dev_list:
        if "ttyACM" in dev or "ttyUSB" in dev:
            ret.append("/dev/" + dev)
    return ret

def run(args):
    """ list or open serial connections to nucleos """

    print "\nPorts found:\n"

    ports = get_available_ports()
    for port in ports:
        print port
    print ""

    if len(ports) == 1:
        print "Connecting to {}".format(ports)
        open_connection(ports[0], baud=args.baud)
        return 0

    selection = raw_input("Which connection? ")
    for port in ports:
        if selection in port:
            open_connection(port, baud=args.baud)
            return 0
    print "'{}' not a valid port".format(selection)

    return 1

def init_args(parser):
    """ initialize arguments for 'serial' sub-command """

    desc = "list or open serial connections to nucleos"
    subp = parser.add_parser(
        "serial", help=desc, description=desc
    )
    subp.set_defaults(handler=run)

    subp.add_argument("-b", "--baud", required=False, type=int, default=115200)

    return 0
