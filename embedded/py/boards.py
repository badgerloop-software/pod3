
"""
Badgerloop - Board Discovery
"""

# built-in
import subprocess
import json

def get_boards():
    """ Return a list of Nucleo boards attached """

    boards = []

    lsblk_raw = subprocess.check_output(["lsblk", "-fJ"])
    lsblk_data = json.loads(lsblk_raw)

    for device in lsblk_data["blockdevices"]:
        if device["label"] and "NODE_" in device["label"]:
            boards.append(device)

    return boards

def run(args):
    """ prints information about connected boards """

    # Print information about 'NODE_' block devices found
    boards = get_boards()
    print ""
    for board in boards:
        print "{}: {}".format(board["name"], board["mountpoint"])
    print "\nBoards found: {}\n".format(len(boards))

    return 0

def init_args(parser):
    """ initialize arguments for 'boards' sub-command """

    subp = parser.add_parser("boards")
    subp.set_defaults(handler=run)

    return 0
