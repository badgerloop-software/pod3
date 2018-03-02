
"""
Badgerloop - Board Discovery
"""

# built-in
import subprocess
import json

def print_boards(boards, tagline="Boards:\n", label=False, mnt=True):
    """ Function to support common task of listing boards"""

    f_str = "{}"
    if label:
        f_str += " ({})"
    else:
        f_str += "{}"
    if mnt:
        f_str += ": {}"

    print tagline
    for board in boards:
        mnt_point = board["mountpoint"]
        if mnt_point is None:
           mnt_point = "(not mounted)"
        _label = board["label"]
        if not label:
            _label = ""
        name = "/dev/" + board["name"]
        print f_str.format(name, _label, mnt_point)
    print ""

def build_link_name(board):
    """ build individual symbolic link name """

    return "FLASH_" + board["label"][5:] + "_" + board["name"]

def get_boards():
    """ Return a list of Nucleo boards attached """

    boards = []

    lsblk_raw = subprocess.check_output(["lsblk", "-fJ"])
    lsblk_data = json.loads(lsblk_raw)
    index = 0

    for device in lsblk_data["blockdevices"]:
        if device["label"] and "NODE_" in device["label"]:
            device["idx"] = index
            device["link"] = build_link_name(device)
            boards.append(device)
            index += 1

    return boards

def get_claimed_boards():
    """ Return a list of boards claimed by the user """

    ret = []
    boards = get_boards()
    for board in boards:
        # TODO
        pass
    return ret

def get_mounted_boards(mounted=True):
    """ Return a list of Nucleo boards already mounted """

    ret = []
    boards = get_boards()
    for board in boards:
        if board["mountpoint"] and mounted:
            ret.append(board)
        elif not board["mountpoint"] and not mounted:
            ret.append(board)
    return ret

def get_unmounted_boards():
    """ Return a list of Nucleo boards not mounted """

    return get_mounted_boards(mounted=False)

def run(args):
    """ prints information about connected boards """

    # Print information about 'NODE_' block devices found
    boards = get_boards()
    print_boards(boards, tagline="", label=True)
    print "Boards found: {}\n".format(len(boards))

    return 0

def init_args(parser):
    """ initialize arguments for 'boards' sub-command """

    desc = "prints information about connected boards"
    subp = parser.add_parser(
        "boards", help=desc, description=desc
    )
    subp.set_defaults(handler=run)

    return 0
