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
        "-dt", type=float, default=1, help="time pace of the simulation in s"
    )
    parser_start.add_argument(
        "-n", type=int, required=True, help="number of simulation frames to compute"
    )

    parser_start.set_defaults(func=solve)

    args = parser.parse_args()
    try:
        args.func(args)
    except AttributeError:
        parser.print_help()


# Compute th simulation with given arguments
def solve(args):
    repo = Repository.Repository()
    supersampling = 20
    sim = repo.start_simulation(args.name, args.method, args.dt)

    if args.n <= sim["t"]:
        print("Finished")
        return

    nx, ny = sim["field"].shape
    dx = sim["dx"]
    dy = sim["dy"]
    solver = flus.SchemeCTCS(
        np.asfortranarray(sim["lastframe"]),
        np.asfortranarray(sim["field"]),
        dx,
        dy,
        args.dt / supersampling,
    )
    print("Solver initialized")

    for _ in range(args.n - sim["t"]):
        solver.step_n(supersampling)
        repo.add_frame(args.name, np.asarray(solver.get_phitdt()))
    print("Finished")


# List simulations
def db_list(args):
    repo = Repository.Repository()
    repo.list_simulations()


def main():
    parse()


if __name__ == "__main__":
    main()
