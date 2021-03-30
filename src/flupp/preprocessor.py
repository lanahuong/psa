#!/usr/bin/env python3

import json, os.path, argparse, sys, pymongo.errors, hashlib, bson, pickle
import numpy as np
import gaussian
from pymongo import MongoClient


def not_implemented():
    sys.exit("Not implemented")


# Add a context and simulation to the database
def add_simulation(field, dimensions, dx, dy, name, wave):

    # Retrive connection informations
    with open("src/db-config.json") as json_file:
        data = json.load(json_file)
        json_file.close()

    # Connect to DB
    client = MongoClient(
        "mongodb://%s:%s@%s/%s"
        % (data["user"], data["pwd"], data["host"], data["name"])
    )

    try:
        db = client[data["name"]]

        binfield = bson.binary.Binary(pickle.dumps(field, protocol=2))
        context = {
            "field": binfield,
            "dimensions": dimensions,
            "dx": dx,
            "dy": dy,
            "name": name,
        }
        context_id = db["Contexts"].insert_one(context).inserted_id

        binwave = bson.binary.Binary(pickle.dumps(wave, protocol=2))
        simulation = {"t": 0, "frames": [binwave], "context": context_id}
        simulation_id = db["Simulations"].insert_one(simulation).inserted_id

        print("New simulation added : %s" % (str(simulation_id)))

    except pymongo.errors.OperationFailure as e:
        print("ERROR: %s" % (e))


# Parse arguments
def parse():
    # Parse arguments
    parser = argparse.ArgumentParser(prog="preprocessor")
    parser.add_argument(
        "json_file", type=str, help="path to json file to configure a simulation"
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

    # If no file path is given print help and exit with error
    if args.json_file == None:
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

    if "image" in field_desc:
        # not_implemented()
        field = np.ones((1000, 1000))
    # Construct field from image
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
    add_simulation(field, dimensions, dx, dy, args.name, wave)


def main():

    args = parse()

    # If the file does not exist exit with error
    if not os.path.exists(args.json_file):
        sys.exit("preprocessor : error : " + args.json_file + " file not found")

    preprocessing(args)


if __name__ == "__main__":
    main()
