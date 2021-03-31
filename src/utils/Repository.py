import json, pymongo.errors, bson, pickle
from pymongo import MongoClient


class Repository:
    def __init__(self):
        # Retrive connection informations
        with open("src/db-config.json") as json_file:
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
