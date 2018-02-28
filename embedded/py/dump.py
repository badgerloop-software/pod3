
"""
Badgerloop - 'objdump' Arbiter 
"""

# built-in
import subprocess
import os

# internal
from . import TOOLCHAIN, EDITOR
from . flash import firmware_available

def run(args):
    """ help user view the objdump output of a particular ELF file """

    elfs = firmware_available(ext=".elf")
    result = 1

    print "Available objdump targets:\n"
    for elf in elfs:
        print elf
    print ""

    choice = raw_input("File to disassemble? ")
    if choice in elfs:
        pass
    elif choice + ".elf" in elfs:
        choice = choice + ".elf"
    else:
        print "'{}' not found".format(choice)
        return result

    # perform dissasembling
    dump_args = [TOOLCHAIN + "objdump", "-D", "bin/" + choice]
    with open(args.file, "w") as outfile:
        result = subprocess.call(dump_args, stdout=outfile)

    # open file in editor
    subprocess.call([EDITOR, args.file])

    # remove file
    if not args.keep:
        os.remove(args.file)

    return result

def init_args(parser):
    """ initialize arguments for 'dump' sub-command """

    desc = "help user view the objdump output of a particular ELF file"
    subp = parser.add_parser(
        "dump", help=desc, description=desc
    )
    subp.set_defaults(handler=run)

    subp.add_argument("--file", required=False, default="dump.txt")
    subp.add_argument("-k", "--keep", required=False, action="store_true")

    return 0
