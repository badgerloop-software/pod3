
"""
Badgerloop - Perform Board Flashing
"""

# copy to mounted board directory (check that it's good first)

def run(args):
    """ """

    print "flash"
    return 0

def init_args(parser):
    """ """

    subp = parser.add_parser("flash")
    subp.set_defaults(handler=run)

    return 0
