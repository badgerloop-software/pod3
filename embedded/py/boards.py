
"""
Badgerloop - Board Discovery
"""

# parse result of `lsblk -fJ`

def run(args):
    """ """

    print "boards"
    return 0

def init_args(parser):
    """ """

    subp = parser.add_parser("boards")
    subp.set_defaults(handler=run)

    return 0
