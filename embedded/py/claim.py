
"""
Badgerloop - Board-claiming Utility
"""

# built-in
import os

# internal
from .boards import get_boards, print_boards, get_mounted_boards

def get_user():
    """ get the current user's username """

    return os.environ["USER"]

def get_claim_file(board):
    """ build the filename corresponding to claim information """

    if not board["mountpoint"]:
        return None
    return board["mountpoint"] + ".claimed"

def claim(board):
    """ attempt to claim an unclaimed board """

    # make sure board is mounted
    if not board["mountpoint"]:
        print "Can't claim board '{}', not mounted".format(board["name"])
        return False

    user = get_user()
    filename = get_claim_file(board)

    # make sure $USER is set
    if not user:
        print "$USER environment variable not set, can't claim board"
        return False

    with open(filename, "w") as claimfile:
        claimfile.write(user)
    print "Writing '{}' to {} (claiming /dev/{})".format(
        user, filename, board["name"]
    )
    return True

def unclaim(board):
    """ attempt to unclaim a claimed board """

    if get_claim_user(board) != get_user():
        print "Can't unclaim '{}', not claimed by you ({})".format(
            board["name"], get_user()
        )
        return False
    os.remove(get_claim_file(board))
    print "unclaimed /dev/{}".format(board["name"])
    return True

def check_claimed(board):
    """ check if a board claim file exists """

    filename = get_claim_file(board)
    if filename and not os.path.exists(filename):
        return False
    elif not filename:
        return False
    return True

def check_claimed_by_me(board):
    """ Simple abstraction for helping with unclaiming """

    if not check_claimed(board):
        return False
    return get_user() == get_claim_user(board)

def get_claim_user(board):
    """ return the user who has claimed the board """

    if not check_claimed(board):
        return None
    with open(get_claim_file(board), "r") as claimfile:
        return claimfile.read()

def get_claimed_boards(boards, unclaimed=False):
    """ Return a list of boards claimed (or unclaimed) by the user """

    ret = []
    for board in boards:
        if check_claimed(board) and not unclaimed:
            ret.append(board)
        elif not check_claimed(board) and unclaimed:
            ret.append(board)
    return ret

def run(args):
    """ 'claim' entry point """

    # get all sets of boards that will be used
    all_boards = get_boards()
    claimed = get_claimed_boards(all_boards)
    unclaimed = get_claimed_boards(all_boards, unclaimed=True) 
    mnted = get_mounted_boards(boards=all_boards)
    uncl_mnted = get_mounted_boards(boards=unclaimed)

    # show already claimed boards
    if len(claimed):
        print_boards(claimed, tagline="Claimed Boards:\n", mnt=True, claimed=True)
    else:
        print "\nNo boards currently claimed\n"

    # check if there are unclaimed, mounted boards
    if not len(uncl_mnted):
        print (
            "No mounted boards available to claim ({}/{} mounted, "
            "{}/{} claimed)"
        ).format(
            len(mnted), len(all_boards), len(claimed), len(mnted)
        )
        if not args.list:
            return 1

    if args.list:
        return 0

    # show which boards are available to claim
    tag = "{} boards available to claim:\n".format(len(uncl_mnted))
    print_boards(uncl_mnted, tagline=tag, mnt=True)

    # attempt to claim board
    choice = raw_input("Claim which board (i.e. 'sda' or 'a')? ")
    found = False
    for board in uncl_mnted:
        if choice in board["name"]:
            claim(board)
            found = True

    if not found:
        print "'{}' was not found in any board names".format(choice)
        return 1
    return 0

def init_args(parser):
    """ initialize arguments for 'claim' sub-command """

    desc = "claim one or more currently mounted boards"
    subp = parser.add_parser(
        "claim", help=desc, description=desc
    )
    subp.set_defaults(handler=run)

    desc = "only list information about claimed boards"
    subp.add_argument(
        "-l", "--list", required=False, action="store_true", help=desc
    )
    desl = "unclaim boards"
    subp.add_argument(
        "-u", "--un-claim", required=False, action="store_true", help=desc
    )

    return 0
