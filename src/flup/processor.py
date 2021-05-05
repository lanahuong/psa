#!/usr/bin/env python3

import json, pathlib, argparse, sys
import numpy as np
import gaussian
from pyevtk.hl import gridToVTK

from picture_loader import *

utils_path = pathlib.Path(__file__).absolute().parents[1] / "src/utils"
sys.path.append(utils_path.__str__())

import Repository


def not_implemented():
    sys.exit("Not implemented")


# Parse arguments
def parse():
    # Parse arguments
    parser = argparse.ArgumentParser(prog="flup")
    subparsers = parser.add_subparsers()

    parser_init = subparsers.add_parser("init")
    parser_init.set_defaults(func=db_initialize)

    parser_reset = subparsers.add_parser("reset")
    parser_reset.set_defaults(func=db_reset)

    parser_new = subparsers.add_parser("new")
    parser_new.add_argument(
        "json_file",
        type=str,
        help="path to json file to configure a simulation",
    )
    parser_new.set_defaults(func=preprocessing)

    parser_list = subparsers.add_parser("list")
    parser_list.set_defaults(func=db_list)

    parser_visual = subparsers.add_parser("visual")
    parser_visual.add_argument(
        "name",
        type=str,
        help="name identifier of a simulation to visualise",
    )
    parser_visual.add_argument(
        "-f", type=int, help="first frame to export to vtk visualize"
    )
    parser_visual.add_argument(
        "-l", type=int, help="last frame to export to vtk visualize"
    )
    parser_visual.add_argument(
        "-p",
        action="store_true",
        help="export only the potential field to vtk visualize",
    )
    parser_visual.set_defaults(func=postprocessing)

    args = parser.parse_args()
    try:
        args.func(args)
    except AttributeError:
        parser.print_help()


# Initialize database
def db_initialize(args):
    repo = Repository.Repository()
    repo.init_db()
    print("The database was correctly initialized.")


# Remove all documents from data base
def db_reset(args):
    repo = Repository.Repository()
    repo.clean_db()
    print("The database has been reset.")


# List simulations
def db_list(args):
    repo = Repository.Repository()
    repo.list_simulations()


def postprocessing(args):
    repo = Repository.Repository()

    minframe = 0
    maxframe = repo.time_simulated(args.name)

    if args.f != None:
        if args.f > maxframe:
            sys.exit("error: the fist frame is after the last frame")
        if args.f < 0:
            sys.exit("error: a frame number should be positive")
        minframe = args.f
    if args.l != None:
        if args.l < 0:
            sys.exit("error: a frame number should be positive")
        if args.l < minframe:
            sys.exit("error: the last frame is before the first frame")
        maxframe = args.l

    sim = repo.get_simulation(args.name, minframe, maxframe)

    nx, ny = sim["field"].shape
    nt = sim["t"] + 1
    half_w = sim["dimensions"][0] / 2
    half_h = sim["dimensions"][1] / 2
    x = np.linspace(-half_w, half_w, nx, dtype=np.float64)
    y = np.linspace(-half_h, half_h, ny, dtype=np.float64)
    z = np.linspace(0.0, 1.0, 1, dtype=np.float64)
    t = np.linspace(0, 2 * np.pi, nt, dtype=np.float64)

    filename = "%s_potential" % args.name
    gridToVTK(
        filename,
        x,
        y,
        z,
        pointData={
            "N": sim["field"].astype(np.float64).reshape((nx, ny, 1), order="C")
        },
    )
    print("%s.vtr generated" % (filename))

    if args.p == True:
        return

    for i, f in enumerate(sim["frames"]):
        filename = "%s_frame%04d" % (args.name, i + minframe)
        gridToVTK(
            filename,
            x,
            y,
            z,
            pointData={
                "N": np.asarray(f.real, order="C").reshape((nx, ny, 1), order="C")
            },
        )
        print("%s.vtr generated" % (filename))


# Preprocessing operations
def preprocessing(args):
    # Decode JSON file
    with open(args.json_file) as json_file:
        data = json.load(json_file)
        # If there is no field description exit with error
        if "field" not in data:
            sys.exit("Missing field description in JSON")
        # If there is no wave description exit with error
        if "wave" not in data:
            sys.exit("Missing wave description in JSON")

        # Get field and wave description
        sim_name = data["name"]
        field_desc = data["field"]
        wave_desc = data["wave"]

        # Close JSON file
        json_file.close()

    repo = Repository.Repository()
    replace = False

    if repo.name_is_in_db(sim_name):
        if repo.simulation_started(sim_name):
            sys.exit(
                "The simulation "
                + sim_name
                + " is already in the database. Choose another name."
            )
        else:
            replace = True

    # Construct field
    if "dimensions" not in field_desc:
        sys.exit("Missing 'dimensions' in field description")

    dimensions = field_desc["dimensions"]

    # Construct field from image
    if "image" in field_desc:
        image_path = field_desc["image"]

        field = load_image(image_path)

    elif "formula" in field_desc:
        field = field_desc["formula"] * np.ones(field_desc["nbSegments"])
    # Construct field from formula
    else:
        sys.exit("Unknown field description use image or formula")

    # Construct wave
    if "type" not in wave_desc:
        sys.exit("Missing 'type' in wave description")

    # Construct wave
    if wave_desc["type"] == "gaussian":
        wave = gaussian.createMapGaussian(wave_desc, dimensions, field.shape)
        # Construct gaussian wave
    elif wave_desc["type"] == "HO":
        not_implemented()
        # Construct HO solution wave
    else:
        sys.exit("Unknown wave type use 'gaussian' or 'HO'")

    dx = dimensions[0] / field.shape[0]
    dy = dimensions[1] / field.shape[1]

    if replace:
        repo.update_simulation(field, dimensions, dx, dy, sim_name, wave)
    else:
        repo.add_simulation(field, dimensions, dx, dy, sim_name, wave)


def main():
    parse()


if __name__ == "__main__":
    main()
