
"""
Badgerloop - '$(toolchain)gdb' Arbiter 
"""

# built-in
import subprocess

def run(args):
    """ help user instantiate a gdb session with a nucleo """

    print "in debug"

    return 0

def init_args(parser):
    """ initialize arguments for 'debug' sub-command """

    desc = "help user instantiate a gdb session with a nucleo"
    subp = parser.add_parser(
        "debug", help=desc, description=desc
    )
    subp.set_defaults(handler=run)

    return 0
