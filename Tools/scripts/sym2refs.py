#!/usr/bin/python3

import os, sys
from datetime import date

def main(args):
    try:
        sym = args[1]
    except IndexError:
        sys.exit(f"Usage: {sys.args[0]} SYM")

    print(f'@ Auto generated by {args[0]} at {date.today()}\n')
    print(".macro fun value, name")
    print("    .global \\name")
    print("    .type \\name, function")
    print("    .set \\name, \\value")
    print(".endm")
    print("")
    print(".macro dat value, name")
    print("    .global \\name")
    print("    .type \\name, object")
    print("    .set \\name, \\value")
    print(".endm")
    print("")

    with open(sym, 'r') as f:
        for line in f.readlines():
            ptr  = f"0x{line[0:8]}"
            name = line[9:-1]

            if eval(ptr) & 1:
                print(f"fun {ptr}, {name}")
            else:
                print(f"dat {ptr}, {name}")

if __name__ == '__main__':
    main(sys.argv)
