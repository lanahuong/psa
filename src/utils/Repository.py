import json, pymongo.errors, bson, pickle, pathlib
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

    # Add a context and simulation to the database
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

            print("New simulation added : %s" % (str(simulation_id)))

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))

    def clean_db(self):

        try:
            db = self._client[self._db_name]

            db.Simulations.remove({})

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))

    def init_db(self):

        try:
            db = self._client[self._db_name]

            simulationsCol = db.create_collection("Simulations")

        except pymongo.errors.OperationFailure as e:
            print("ERROR: %s" % (e))
