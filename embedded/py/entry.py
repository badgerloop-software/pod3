
""" 
Badgerloop - Build Interface Entry (main)
"""

# built-in
import argparse

# internal
from . import boards
from . import flash
from . import mount
from . import serial
from . import dump
from . import debug

DESC = "Badgerloop Embedded Development Utility"

def main(argv):
    """ build.py entry """

    # Run interactively if no arguments are supplied
    interactive = False
    if len(argv) == 1:
        interactive = True

    # Initialize argparse
    parser = argparse.ArgumentParser(
        description=DESC,
        prog="build.py"
    )
    subparser = parser.add_subparsers()

    # Add sub-commands
    boards.init_args(subparser)
    flash.init_args(subparser)
    mount.init_args(subparser)
    serial.init_args(subparser)
    dump.init_args(subparser)
    debug.init_args(subparser)

    # Run once in 'batch' mode
    if not interactive:
        args = parser.parse_args(argv[1:])
        return args.handler(args)

    # Continuously run in interactive mode
    print "{} ('help' for commands)".format(DESC)
    command = raw_input("=> ")
    while command != "exit":

        args = command.split()
        if len(args) and args[0] != "":
            if args[0] == "help":
                args[0] = "-h"
            try:
                args = parser.parse_args(args)
                result = args.handler(args)
                if result:
                    print result
            except SystemExit as exc:
                pass

        command = raw_input("=> ")

    return 0
