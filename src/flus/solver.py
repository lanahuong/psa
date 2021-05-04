#!/usr/bin/env python3

import json, pathlib, argparse, sys
import numpy as np

utils_path = pathlib.Path(__file__).absolute().parents[1] / "src/utils"
sys.path.append(utils_path.__str__())

import Repository

# Parse arguments
def parse():
    # Parse arguments
    parser = argparse.ArgumentParser(prog="flus")
    subparsers = parser.add_subparsers()

    parser_list = subparsers.add_parser("list")
    parser_list.set_defaults(func=db_list)

    parser_start = subparsers.add_parser("start")
    parser_start.add_argument(
        "name",
        type=str,
        help="name identifier of a simulation to start",
    )
    parser_start.add_argument(
        "-m",
        "--method",
        type=str,
        choices=["ctcs", "ftcs", "btcs"],
        default="ctcs",
        help="scheme used to compute the simulation",
    )
    parser_start.add_argument("-dt", type=int, help="time pace of the simulation in ms")
    parser_start.add_argument("-t", type=int, help="time of the simulation in ms")

    parser_start.set_defaults(func=solve)

    args = parser.parse_args()
    try:
        args.func(args)
    except AttributeError:
        parser.print_help()


# Compute th simulation with given arguments
def solve(args):
    repo = Repository.Repository()
    sim = repo.get_simulation(args.name)
    # Simulate stuff...


# List simulations
def db_list(args):
    repo = Repository.Repository()
    repo.list_simulations()


def main():
    parse()


if __name__ == "__main__":
    main()
