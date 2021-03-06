
"""
Badgerloop - File System Mounting
"""

# built-in
import os
import subprocess
from time import sleep

# internal
from . import MNT_POINTS
from .boards import get_boards, get_unmounted_boards, get_mounted_boards
from .boards import print_boards
from .claim import claim, unclaim, check_claimed_by_me

def mount(link, mnt_point):
    """ run 'mount' on given mount point and create symbolic link """

    if not os.path.exists(mnt_point):
        os.makedirs(mnt_point)
    sleep(0.25)
    os.symlink(mnt_point, link)
    sleep(0.25)
    return subprocess.call(["mount", link])

def mount_handler(board, brd_mnt_point, mnt_point, claim_board=True):
    """ Perform path checking and printing based on result of mount """

    if not os.path.exists(board["link"]) and not brd_mnt_point:
        result = mount(board["link"], mnt_point)
        if result != 0:
            print "mount failed"
            return result
        print "mounted /dev/{} to {}".format(board["name"], board["link"])
        board["mountpoint"] = mnt_point
        if claim_board:
            sleep(0.25)
            claim(board)
    return None

def unmount(link, mnt_point):
    """ run 'umount' on given mount point and remove link """

    result = subprocess.call(["umount", mnt_point])
    if result == 0:
        os.remove(link)
        os.rmdir(mnt_point)
    return result

def unmount_handler(board, mnt_point):
    """ Perform path checking and printing based on result of unmount """

    if os.path.exists(board["link"]) and unmount(board["link"], mnt_point) == 0:
        print "umounted /dev/{}".format(board["name"])
        if check_claimed_by_me(board):
            unclaim(board)
        print ""

def run(args):
    """
    attempt to mount nucleos to MNT_POINTS[i] and symbolically
    link them to cwd (named from 'build_link_name')
    """

    # Create symbolic links to mount points
    boards = get_unmounted_boards()

    if not args.all:

        # prompt to unmount board
        if args.nmount:
            boards = get_mounted_boards()
            if not len(boards):
                print "No mounted boards found"
                return 1
            print_boards(boards, tagline="Mounted boards:\n")
            choice = raw_input("Unmount which board (i.e. 'sda' or 'a')? ")
            for board in boards:
                if choice in board["name"]:
                    unmount_handler(board, board["mountpoint"])
            return 0

        if not len(boards):
            print "all boards mounted or none found"
            return 1

        # display options (boards)
        print_boards(boards, tagline="\nUnmounted boards:\n")

        # prompt for choice
        choice = raw_input("Mount which board (i.e. 'sda' or 'a')? ")
        for idx, board in enumerate(boards):
            if choice in board["name"]:
                # mount board if allowed
                result = mount_handler(
                    board, board["mountpoint"], MNT_POINTS[board["idx"]]
                )
                if result:
                    return result
                print ""
                return 0

        print "'{}' couldn't be matched".format(choice)
        return 1

    # 'mount' or 'umount' all possible boards
    boards = get_boards()
    for board in boards:

        # umount
        if args.nmount:
            unmount_handler(board, MNT_POINTS[board["idx"]])
            continue

        # mount
        result = mount_handler(
            board, board["mountpoint"], MNT_POINTS[board["idx"]]
        )
        if result:
            return result

    return 0

def init_args(parser):
    """ initialize arguments for 'mount' sub-command """

    subp = parser.add_parser(
        "mount", description="mount and umount nucleos",
        help="attemp to mount nucleos to filesystem based on /etc/fstab entries"
    )
    subp.add_argument(
        "-u", "--nmount", action="store_true", required=False,
        help="umount instead of mount"
    )
    subp.add_argument(
        "-a", "--all", help="mount all available boards", required=False,
        action="store_true"
    )
    subp.set_defaults(handler=run)

    return 0
