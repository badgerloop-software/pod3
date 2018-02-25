
"""
Badgerloop - Board Discovery
"""

# built-in
import subprocess
import json

# parse result of `lsblk -fJ`

def run(args):
    """ """

    board_count = 0
    lsblk_raw = subprocess.check_output(["lsblk", "-fJ"])
    lsblk_data = json.loads(lsblk_raw)

    for device in lsblk_data["blockdevices"]:
        if device["label"] and "NODE_" in device["label"]:
            print device["label"]
            board_count += 1

    print "Boards found: %d" % board_count

    return 0

def init_args(parser):
    """ """

    subp = parser.add_parser("boards")
    subp.set_defaults(handler=run)

    return 0
