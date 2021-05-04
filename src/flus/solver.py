#!/usr/bin/env python3

import json, pathlib, argparse, sys
import numpy as np

utils_path = pathlib.Path(__file__).absolute().parents[1] / "src/utils"
bindings_path = pathlib.Path(__file__).absolute().parents[1] / "bindings"
sys.path.append(utils_path.__str__())
sys.path.append(bindings_path.__str__())

import Repository
import flus

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
    parser_start.add_argument(
        "-dt", type=float, default=0.1, help="time pace of the simulation in ms"
    )
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
    # Simulate stuff...

    sim = repo.start_simulation(args.name, args.method, args.dt)

    nx, ny = sim["field"].shape
    dx = sim["dx"]
    dy = sim["dy"]
    solver = flus.SchemeCTCS(
        np.asfortranarray(sim["lastframe"]),
        np.asfortranarray(sim["field"]),
        dx,
        dy,
        args.dt,
    )
    print("Solver initialized")

    for _ in range(20):
        solver.step()
        repo.add_frame(args.name, np.asarray(solver.get_state()))
    print("Finished")


# List simulations
def db_list(args):
    repo = Repository.Repository()
    repo.list_simulations()


def main():
    parse()


if __name__ == "__main__":
    main()
