
"""
Badgerloop - Serial Port Interaction
"""

# built-in
import subprocess

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

    return 0

def init_args(parser):
    """ initialize arguments for 'serial' sub-command """

    desc = "list or open serial connections to nucleos"
    subp = parser.add_parser(
        "serial", help=desc, description=desc
    )
    subp.set_defaults(handler=run)

    return 0
