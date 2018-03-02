
"""
Badgerloop - Board-claiming Utility
"""

def run(args):
    """ """

    print "claim"
    return 0

def init_args(parser):
    """ """

    desc = "claim one or more currently mounted boards"
    subp = parser.add_parser(
        "claim", help=desc, description=desc
    )
    subp.set_defaults(handler=run)

    return 0
