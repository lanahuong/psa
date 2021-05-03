import json, pymongo.errors, bson, pickle, pathlib, sys
from pymongo import MongoClient

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
                "frames": [binwave],
            }

            simulation_id = db["Simulations"].insert_one(simulation).inserted_id

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
                "frames": [binwave],
            }

            simulation_id = (
                db["Simulations"].replace_one({"name": name}, simulation).upserted_id
            )

            print("Simualation updated : %s" % (name))

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

    # Print a list of the simulations in the database
    def list_simulations(self):
        try:
            db = self._client[self._db_name]

            for document in db.Simulations.find({}):
                print(document["name"])

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))

    def get_simulation(self, name):
        try:
            db = self._client[self._db_name]

            document = db.Simulations.find_one({"name": name})

            if document == None:
                sys.exit(
                    "The simulation "
                    + name
                    + " name is not in the database. Check spelling."
                )

            field = pickle.loads(document["field"])
            document["field"] = field

            frames = []
            for f in document["frames"]:
                frames.append(pickle.loads(f))
            document["frames"] = frames

            return document

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))

    # Clean the database (make it empty but collections still exists)
    def clean_db(self):

        try:
            db = self._client[self._db_name]

            db.Simulations.remove({})

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))

    # Create needed collections in the database
    def init_db(self):

        try:
            db = self._client[self._db_name]

            simulationsCol = db.create_collection("Simulations")

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))
