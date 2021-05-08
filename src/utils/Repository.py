import json, pymongo.errors, bson, pickle, pathlib, sys
from pymongo import MongoClient

# from tabulate import tabulate

utils_path = pathlib.Path(__file__).absolute().parents[2] / "src/utils"


class Repository:
    def __init__(self):
        # Retrive connection informations
        with open(utils_path / "db-config.json") as json_file:
            data = json.load(json_file)
            json_file.close()

        # Connect to DB
        self._client = MongoClient(
            "mongodb://%s:%s@%s/%s"
            % (data["user"], data["pwd"], data["host"], data["name"])
        )

        self._db_name = data["name"]

    # Add a simulation to the database
    def add_simulation(self, field, dimensions, dx, dy, name, wave):

        try:
            db = self._client[self._db_name]

            binfield = bson.binary.Binary(pickle.dumps(field, protocol=2))
            binwave = bson.binary.Binary(pickle.dumps(wave, protocol=2))
            simulation = {
                "name": name,
                "dimensions": dimensions,
                "field": binfield,
                "t": 0,
                "dx": dx,
                "dy": dy,
                "method": "",
            }
            frame = {"name": name, "num": 0, "phi": binwave}

            db["Simulations"].insert_one(simulation)
            db["Frames"].insert_one(frame)

            print("New simulation added : %s" % (name))

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))

    # Update a simulation to the database
    def update_simulation(self, field, dimensions, dx, dy, name, wave):

        try:
            db = self._client[self._db_name]

            binfield = bson.binary.Binary(pickle.dumps(field, protocol=2))
            binwave = bson.binary.Binary(pickle.dumps(wave, protocol=2))
            simulation = {
                "name": name,
                "dimensions": dimensions,
                "field": binfield,
                "t": 0,
                "dx": dx,
                "dy": dy,
                "method": "",
            }
            frame = {"name": name, "num": 1, "phi": binwave}

            db["Simulations"].replace_one({"name": name}, simulation)
            db["Frames"].replace_one({"name": name, "num": 1}, frame)

            print("Simualation updated : %s" % (name))

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))

    def start_simulation(self, name, method, dt):
        try:
            db = self._client[self._db_name]

            sim = db.Simulations.find_one({"name": name})
            if sim == None:
                sys.exit("error: no simulation named " + name + "in the database.")
            if sim["method"] != "" and method != sim["method"]:
                sys.exit(
                    "error: the simulation was already started with a different method"
                )
            if "dt" in sim and dt != sim["dt"]:
                sys.exit(
                    "error: the simulation was already started with a different time pace."
                )

            db["Simulations"].update_one(
                {"name": name}, {"$set": {"method": method, "dt": dt}}
            )

            sim = db.Simulations.find_one({"name": name})

            field = pickle.loads(sim["field"])
            sim["field"] = field

            lastframe = db.Frames.find_one({"name": name, "num": sim["t"]})
            sim["lastframe"] = pickle.loads(lastframe["phi"])

            print("Starting simulation %s" % (name))

            return sim

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))

    def add_frame(self, name, frame):
        try:
            db = self._client[self._db_name]

            binwave = bson.binary.Binary(pickle.dumps(frame, protocol=2))

            db["Simulations"].update_one({"name": name}, {"$inc": {"t": 1}})
            sim = db["Simulations"].find_one({"name": name})

            frame = {"name": name, "num": sim["t"], "phi": binwave}
            db["Frames"].insert_one(frame)

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))

    # Check if a name is in the database
    def name_is_in_db(self, name):
        try:
            db = self._client[self._db_name]

            return db.Simulations.find_one({"name": name}) != None

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))

    # Check if a name is in the database
    def simulation_started(self, name):
        try:
            db = self._client[self._db_name]

            document = db.Simulations.find_one({"name": name})

            if document == None:
                sys.exit("name not found")

            return document["t"] > 0

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))

    def time_simulated(self, name):
        try:
            db = self._client[self._db_name]

            document = db.Simulations.find_one({"name": name})

            if document == None:
                sys.exit("name not found")

            return document["t"]

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))

    # Print a list of the simulations in the database
    def list_simulations(self):
        try:
            db = self._client[self._db_name]

            data = []
            for doc in db.Simulations.find({}):
                data.append([doc["name"], doc["t"], doc["method"]])

            headers = ["Name", "Frames", "Scheme"]
            row_format = "{:>15}" * (len(headers))
            print(row_format.format(*headers))
            for row in data:
                print(row_format.format(*row))

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))

    def get_simulation(self, name, first, last):
        try:
            db = self._client[self._db_name]

            sim = db.Simulations.find_one({"name": name})

            if sim == None:
                sys.exit(
                    "The simulation "
                    + name
                    + " name is not in the database. Check spelling."
                )

            field = pickle.loads(sim["field"])
            sim["field"] = field

            frames = db.Frames.find(
                {
                    "name": name,
                },
                projection=["num", "phi"],
                # sort=[("num", pymongo.ASCENDING)],
            )
            sim["frames"] = []
            for i in range(first, last + 1):
                f = db.Frames.find_one({"name": name, "num": i}, projection=["phi"])
                sim["frames"].append(pickle.loads(f["phi"]))

            return sim

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))

    # Clean the database (make it empty but collections still exists)
    def clean_db(self):

        try:
            db = self._client[self._db_name]

            db.Simulations.drop()
            db.Frames.drop()

            self.init_db()

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))

    # Create needed collections in the database
    def init_db(self):

        try:
            db = self._client[self._db_name]

            simulationsCol = db.create_collection("Simulations")
            framesCol = db.create_collection("Frames")

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))
