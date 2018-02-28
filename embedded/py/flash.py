
"""
Badgerloop - Perform Board Flashing
"""

# built-in
import subprocess
import os

# interal
from .boards import get_boards

def run(args):
    """ prompt user to flash firmware to mounted boards """

    # check for 'bin' directory
    if not os.path.exists("bin"):
        print "No 'bin' directory containing firmware"
        return 1

    # find .bin firmware options
    print "\nAvailable firmware:\n"
    for obj in subprocess.check_output(["ls", "bin"]).split():
        if ".bin" in obj:
            print obj
    print""

    # print number of boards available
    boards = get_boards()
    print "{} boards available for flashing\n".format(len(boards))

    for board in boards:
        if board["mountpoint"] is not None:
            firmware = raw_input("Enter firmware for /dev/{} ({}): ".format(
                board["name"], board["label"][5:]
            ))
            if not os.path.exists("bin/" + firmware):
                if not os.path.exists("bin/" + firmware + ".bin"):
                    print "{} and {}.bin not found in 'bin'".format(
                        firmware, firmware
                    )

                # flash {firmware}.bin
                else:
                    print "flashing {}.bin to {}".format(firmware, board["mountpoint"])
                    subprocess.call(["cp", "bin/" + firmware + ".bin", board["mountpoint"]])

            # flash {firmware}
            else:
                print "flashing {} to {}".format(firmware, board["mountpoint"])
                subprocess.call(["cp", "bin/" + firmware, board["mountpoint"]])

            # ask OS to flush this write immediately
            subprocess.call(["sync"])

        else:
            print "board at '/dev/{}' ({}) not mounted, skipping".format(
                board["name"], board["label"][5:]
            )

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
