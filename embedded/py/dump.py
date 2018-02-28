
"""
Badgerloop - 'objdump' Arbiter 
"""

# built-in
import subprocess

def run(args):
    """ help user view the objdump output of a particular ELF file """

    print "in dump"

    return 0

def init_args(parser):
    """ initialize arguments for 'dump' sub-command """

    desc = "help user view the objdump output of a particular ELF file"
    subp = parser.add_parser(
        "dump", help=desc, description=desc
    )
    subp.set_defaults(handler=run)

    return 0
