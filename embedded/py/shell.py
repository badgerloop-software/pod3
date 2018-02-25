
"""
Badgerloop - Shell Execution API
"""

# simple use of the shell exec utilities

def run(args):
    """ """

    print "shell"
    return 0

def init_args(parser):
    """ initialize arguments for 'shell' sub-command """

    subp = parser.add_parser("shell")
    subp.set_defaults(handler=run)

    return 0
