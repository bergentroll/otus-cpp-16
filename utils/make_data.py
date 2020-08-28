#! /usr/bin/env python3

from random import randrange, uniform

import getopt, sys


def generate_line(raw=True):
    latitude = uniform(-90, 90)
    longitude = uniform(0, 180)
    rooms = randrange(1, 6)
    price = uniform(1000000, 10000000)
    square = uniform(25, 125)
    kitchen = uniform(square / 5, square / 2)
    storeys = randrange(1, 101)
    floor = randrange(1, storeys + 1)

    result = \
        f"{latitude:.6f};{longitude:.6f};" + \
        f"{rooms};{price:.2f};" + \
        f"{square:.2f};{kitchen:.2f}"
    if raw:
        result += f";{floor};{storeys}"
    else:
        is_middle: bool = floor not in (1, storeys)
        result += f";{int(is_middle)}"

    print(result)


if __name__ == '__main__':
    lines_num = 100
    raw = False

    help_line = 'make_data.py -i <inputfile> -o <outputfile>'

    try:
        opts, args = getopt.getopt(sys.argv[1:], "hn:r")
    except getopt.GetoptError:
        print(help_line)
        sys.exit(1)

    for opt, arg in opts:
        if opt == '-h':
            print(help_line)
            sys.exit()
        elif opt == '-n':
            lines_num = int(arg)
        elif opt == '-r':
            raw = True

    for i in range(lines_num):
        generate_line(raw)
