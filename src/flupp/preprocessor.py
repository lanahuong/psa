#!/usr/bin/env python3
import gaussian
import json, os.path, argparse, sys

def not_implemented():
    sys.exit("Not implemented")

# Parse arguments
def parse():
    # Parse arguments
    parser = argparse.ArgumentParser(prog='preprocessor')
    parser.add_argument("json_file", type=str, help="path to json file to configure a simulation")
    args = parser.parse_args()

    # If no file path is given print help and exit with error
    if (args.json_file == None):
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
        field = data["field"]
        wave = data["wave"]

        # Close JSON file
        json_file.close()

    # Construct field
    if "dimensions" not in field:
        sys.exit("Missing 'dimensions' in field description")

    dimensions = field["dimensions"]

    #if "image" in field:
        #not_implemented()
        # Construct field from image
    #elif "formula" in field:
        #not_implemented()
        # Construct field from formula
    #else:
        #sys.exit("Unknown field description use image or formula")

    # Construct wave
    if "type" not in wave:
        sys.exit("Missing 'type' in wave description")

    # Construct wave
    if wave["type"] == "gaussian":
        initGauss = gaussian.createMapGaussian(wave , field)
        # Construct gaussian wave
    elif wave["type"] == "HO":
        not_implemented()
        # Construct HO solution wave
    else:
        sys.exit("Unknown wave type use 'gaussian' or 'HO'")


def main():

    args = parse()

    # If the file does not exist exit with error
    if  not os.path.exists(args.json_file):
        sys.exit("preprocessor : error : "+args.json_file+" file not found")

    preprocessing(args)



if __name__ == '__main__':
    main()
