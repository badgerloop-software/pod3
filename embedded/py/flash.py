
"""
Badgerloop - Perform Board Flashing
"""

# built-in
import subprocess
import os

# interal
from .boards import get_boards, get_mounted_boards

def firmware_available(ext=".bin", _dir="bin"):
    """ find all available firmware binaries """

    found = []

    # check for 'bin' directory
    if not os.path.exists(_dir):
        return found

    # collect elements from 'ls'
    for obj in subprocess.check_output(["ls", "bin"]).split():
        if ext in obj:
            found.append(obj)
    return found

def prompt_for_binary(boards, ask_first=True, ext=".bin"):
    """ """

    matches = {}

    for board in boards:

        firmware = ""

        # make sure board is mounted
        if board["mountpoint"] is not None:

            curr_board = "/dev/{} ({})".format(
                board["name"], board["label"][5:]
            )

            # simple y/n prompt to flash board first
            if ask_first:
                prompt = "Write firmware to {}? [y/n] ".format(curr_board)
                ask_flash = raw_input(prompt)
                ask_flash = ask_flash.lower()
                if "y" not in ask_flash:
                    continue

                # prompt user to enter name of binary
                firmware = raw_input("Choice: ")

            # prompt to flash each board
            else:
                prompt = "Enter firmware for {}: ".format(curr_board)
                firmware = raw_input(prompt)

            if not firmware:
                continue

            # search for "firmware"
            if not os.path.exists("bin/" + firmware):

                # search for "firmware.bin"
                if not os.path.exists("bin/" + firmware + ext):
                    print "{} and {}{} not found in 'bin'".format(
                        firmware, firmware, ext
                    )
                else:
                    matches[board["mountpoint"]] = firmware + ext
            else:
                matches[board["mountpoint"]] = firmware

        # if not mounted, report to user
        else:
            print "board at '/dev/{}' ({}) not mounted, skipping".format(
                board["name"], board["label"][5:]
            )

    return matches

def run(args):
    """ prompt user to flash firmware to mounted boards """

    # find available firmware
    firmware = firmware_available()
    if not len(firmware):
        print "No 'bin' directory containing firmware"
        return 1

    print "\nAvailable firmware:\n"
    for binary in firmware:
        print binary
    print""

    # print number of boards available
    # TODO: check claimed boards
    boards = get_mounted_boards()
    print "{} boards available for flashing\n".format(len(boards))

    # prompt user for firmware selections per mounted board
    matches = prompt_for_binary(boards)
    print ""

    # write the firmware to the board
    firm_written = False
    for mnt in matches:
        print "flashing {} to {}".format(matches[mnt], mnt)
        subprocess.call(["cp", "bin/" + matches[mnt], mnt])
        firm_written = True

    # ask OS to flush these write immediately
    if firm_written:
        subprocess.call(["sync"])
        print ""

    return 0

def init_args(parser):
    """ initialize arguments for 'flash' sub-command """

    desc = "prompt user to flash firmware to mounted boards"
    subp = parser.add_parser(
        "flash", help=desc, description=desc
    )
    subp.set_defaults(handler=run)

    return 0
