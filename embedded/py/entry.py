
""" 
Badgerloop - Build Interface Entry (main)
"""

def main(argv):

    print argv

    while True:

        command = raw_input("=> ")

        # TODO: scope commands and set up argparse

        # TODO: remove these
        if command == "exit":
            return 0
        elif command == "fail":
            return 1
