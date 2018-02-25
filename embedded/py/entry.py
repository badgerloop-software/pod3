
""" 
Badgerloop - Build Interface Entry (main)
"""

# built-in
import argparse

# internal
from . import boards
from . import flash
from . import mount

DESC = "Badgerloop Firmware Flashing Utility"

def main(argv):
    """ build.py entry """

    # Run interactively if no arguments are supplied
    interactive = False
    if len(argv) == 1:
        interactive = True

    # Initialize argparse
    parser = argparse.ArgumentParser(
        description=DESC,
        epilog="University of Wisconsin-Madison Undergraduates\n",
        prog="build.py"
    )
    subparser = parser.add_subparsers()

    # Add sub-commands
    boards.init_args(subparser)
    flash.init_args(subparser)
    mount.init_args(subparser)

    # Run once in 'batch' mode
    if not interactive:
        args = parser.parse_args(argv[1:])
        return args.handler(args)

    # Continuously run in interactive mode
    print "{} ('-h' for commands)".format(DESC)
    command = raw_input("=> ")
    while command != "exit":

        args = command.split()
        if len(args) and args[0] != "":
            try:
                args = parser.parse_args(args)
                result = args.handler(args)
                if result:
                    print result
            except SystemExit as exc:
                pass

        command = raw_input("=> ")

    return 0
