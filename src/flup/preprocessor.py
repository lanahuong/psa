#!/usr/bin/env python3

import json, pathlib, argparse, sys
import numpy as np
import gaussian

from picture_loader import *

utils_path = pathlib.Path(__file__).absolute().parents[1] / "src/utils"
sys.path.append(utils_path.__str__())

import Repository


def not_implemented():
    sys.exit("Not implemented")


# Parse arguments
def parse():
    # Parse arguments
    parser = argparse.ArgumentParser(prog="preprocessor")
    parser.add_argument("action", type=str, choices=["init", "reset", "new"])
    parser.add_argument(
        "json_file",
        type=str,
        nargs="?",
        help="path to json file to configure a simulation",
    )
    parser.add_argument(
        "--name",
        "-n",
        default="",
        nargs="?",
        type=str,
        dest="name",
        help="name of the configuration",
    )
    args = parser.parse_args()

    if args.action == None:
        parser.print_help()
        sys.exit()

    # If no file path is given print help and exit with error
    if args.action == "new" and args.json_file == None:
        parser.print_help()
        sys.exit("Missing json file to configure")

    return args


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
        field_desc = data["field"]
        wave_desc = data["wave"]

        # Close JSON file
        json_file.close()

    # Construct field
    if "dimensions" not in field_desc:
        sys.exit("Missing 'dimensions' in field description")

    dimensions = field_desc["dimensions"]

    # Construct field from image
    if "image" in field_desc:
        image_path = field_desc["image"]

        field = load_image(image_path)
        print(field)

    elif "formula" in field:
        not_implemented()
    # Construct field from formula
    else:
        sys.exit("Unknown field description use image or formula")

    # Construct wave
    if "type" not in wave_desc:
        sys.exit("Missing 'type' in wave description")

    # Construct wave
    if wave_desc["type"] == "gaussian":
        wave = gaussian.createMapGaussian(wave_desc, field_desc)
        # Construct gaussian wave
    elif wave_desc["type"] == "HO":
        not_implemented()
        # Construct HO solution wave
    else:
        sys.exit("Unknown wave type use 'gaussian' or 'HO'")

    dx = dimensions[0] / field.shape[0]
    dy = dimensions[1] / field.shape[1]

    repo = Repository.Repository()
    repo.add_simulation(field, dimensions, dx, dy, args.name, wave)


def main():

    args = parse()

    if args.action == "init":
        repo = Repository.Repository()
        repo.init_db()
        print("The database was correctly initialized.")
    elif args.action == "reset":
        repo = Repository.Repository()
        repo.clean_db()
        print("The database has been reset.")
    elif args.action == "new":
        # If the file does not exist exit with error
        if not pathlib.Path(args.json_file).exists():
            sys.exit("preprocessor : error : " + args.json_file + " file not found")

    preprocessing(args)


if __name__ == "__main__":
    main()
