
"""
Badgerloop - '$(toolchain)gdb' Arbiter 
"""

# built-in
import subprocess
import os
from time import sleep

# internal
from . import TOOLCHAIN
from .boards import get_boards
from . flash import firmware_available

ERR_MSG = "couldn't find 'st-util', download it at: {} and {}".format(
    "https://github.com/texane/stlink",
    "follow the instructions in doc/compiling.md"
)

def run(args):
    """ help user instantiate a gdb session with a nucleo """

    # make sure 'st-util' is available
    st_util_present = subprocess.call(
        ["which", "st-util"], stdout=open(os.devnull, 'wb')
    )
    if st_util_present:
        print ERR_MSG
        return 1

    # make sure $(TOOLCHAIN)gdb is available
    gdb_present = subprocess.call(
        ["which", TOOLCHAIN + "gdb"], stdout=open(os.devnull, 'wb')
    )
    if gdb_present:
        print "'{}' not available and is required".format(TOOLCHAIN + "gdb")
        return 1

    num_boards = len(get_boards())
    if num_boards > 1:
        print (
            "Using 'st-util' with more than one board is not deterministic."
            " Unplug {} and try again.".format(num_boards - 1)
        )
        return 1

    # list programs that may be debugged
    elfs = firmware_available(ext=".elf")
    print "Program needs to be based on one of the following compilations:\n"
    for elf in elfs:
        print elf
    print ""

    # prompt user for selection
    choice = raw_input("Which program is running? ")
    if choice in elfs:
        pass
    elif choice + ".elf" in elfs:
        choice = choice + ".elf"
    else:
        print "'{}' not found".format(choice)
        return 1

    # run GDB server and connect to it with GDB
    proc = subprocess.Popen(["st-util"], stdout=open(os.devnull, 'wb'))
    sleep(2)
    subprocess.call([TOOLCHAIN + "gdb", "bin/" + choice])
    proc.terminate()

    return 0

def init_args(parser):
    """ initialize arguments for 'debug' sub-command """

    desc = "help user instantiate a gdb session with a nucleo"
    subp = parser.add_parser(
        "debug", help=desc, description=desc
    )
    subp.set_defaults(handler=run)

    return 0
