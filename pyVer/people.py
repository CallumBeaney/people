#!/usr/bin/python3

import sys
import argparse

# TODO: https://gist.github.com/umangahuja1/51da3a453803f1f67f4eee5de129d4db

class ErrorCheckingParser(argparse.ArgumentParser):
    def error(self, message):
        sys.stderr.write('error: %s\n' % message)
        self.print_help()
        sys.exit(2)

parser = ErrorCheckingParser()
subparsers = parser.add_subparsers(help="commands")

# ADD A PERSON+DATE TO THE DATABASE
add_parser = subparsers.add_parser('add', help='add a person')
add_parser.add_argument('add', nargs=1) # ? == a single value which is optional
add_parser.add_argument('relationship', nargs=1) 

# FORGET A PERSON (REMOVE THEM FROM THE DATABASE)
forget_parser = subparsers.add_parser('forget', help="forget a person")
forget_parser.add_argument('forget')

# CHECK ALL PEOPLE, A PERSON, OR A TYPE OF CONNECTION IN THE DATABASE 
check_parser = subparsers.add_parser('check', help="check in on all people, a specific name, or a relationship type")
check_parser.add_argument('check', choices=['all', 'name', 'type'], nargs='?')
check_parser.add_argument('additional_arg', nargs='?', default=None)
# Modify the nargs parameter for the 'name' choice
name_parser = subparsers.add_parser('name')
name_parser.add_argument('name', nargs=1)
# Modify the nargs parameter for the 'type' choice
type_parser = subparsers.add_parser('type')
type_parser.add_argument('type', nargs=1)


argsNamespace = parser.parse_args()
args = vars(argsNamespace)
print(args)




# parser.add_argument('command', choices=['add', 'check', 'forget', 'days'], help='Type of action to be be performed')
# parser.add_argument('check_lookup', choices=['all', 'name', 'type'], required="check")
# parser.add_argument('lookup_data', type=str, help='Name or other data to be added or checked')
# args = parser.parse_args()

# match args.command:
#     case 'add':
#         print(f"adding {args.lookup_data} in the CSV")
#         addPerson(args.lookup_data)
#     case 'check':
#         print(f"Checking {args.lookup_data} in the CSV")
#         checkSomething(args.lookup_data)
#     case 'forget':
#         print(f"Forgetting {args.lookup_data} from the list")
#     case 'days':
#         print(f"Changing day check {args.lookup_data} to program settings")



