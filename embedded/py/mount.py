
"""
Badgerloop - File System Mounting
"""

# built-in
import os
import subprocess
from time import sleep

# internal
from .boards import get_boards

# Only support four concurrent devices for now
MNT_POINTS = [
    "/tmp/nucleo1", "/tmp/nucleo2",
    "/tmp/nucleo3", "/tmp/nucleo4"
]

def build_link_name(board):
    """ build individual symbolic link name """

    return "FLASH_" + board["label"][5:] + "_" + board["name"]

def build_links(_boards):
    """
    from _boards dictionaries, create a list of
    symbolic link names (used for flashing)
    """

    dirs = []
    for board in _boards:
        dirs.append(build_link_name(board))
    return dirs

def run(args):
    """
    attempt to mount nucleos to MNT_POINTS[i] and symbolically
    link them to cwd (named from 'build_link_name')
    """

    # Create symbolic links to mount points
    boards = get_boards()
    links = build_links(boards)

    # 'mount' or 'umount' depending on input arguments
    for idx, link in enumerate(links):

        # mount
        if not os.path.exists(link) and not args.nmount and not boards[idx]["mountpoint"]:
            if not os.path.exists(MNT_POINTS[idx]):
                os.makedirs(MNT_POINTS[idx])
            sleep(0.25)
            os.symlink(MNT_POINTS[idx], link)
            sleep(0.25)
            result = subprocess.call(["mount", link])
            if result != 0:
                print "mount failed"
                return result
            print "mounted /dev/{} to {}".format(boards[idx]["name"], link)

        # umount
        elif args.nmount and os.path.exists(link):
            result = subprocess.call(["umount", MNT_POINTS[idx]])
            if result == 0:
                os.remove(link)
                os.rmdir(MNT_POINTS[idx])
                print "umounted /dev/{}".format(boards[idx]["name"])

    return 0

def init_args(parser):
    """ initialize arguments for 'mount' sub-command """

    subp = parser.add_parser(
        "mount", description="mount and umount nucleos"
    )
    subp.add_argument(
        "-u", "--nmount", action="store_true", required=False,
        help="umount instead of mount"
    )
    subp.set_defaults(handler=run)

    return 0
