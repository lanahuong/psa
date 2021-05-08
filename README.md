donc# Advanced scientific programming project : 2D-FD solver for the the time-dependent non-relativistic Schrödinger equation

## Authors

BOITEAU Mathis

MIGDAL Michel

SCRAVAGLIERI Lana

## Presentation

This solver is composed of 2 programs the solver `flus` and the processor `flup`.

The processor is used to create and field of potential and the wave at time t=0,
monitor the solver, visualize results of a simulation and initialize or reset the database.

The solver computes the wave at each time step using the previous wave.


## Requirements

- MongoDB
- numpy (python)
- pyevtk  
- pymongo (python)
- armadillo

## Set up

After installing the requirements, create a MongoDB database called `flu_db` and
user of the database with the "readWrite" role called `flu` with password `flupwd`.

If you wish to use other names and password you will need to change the
`src/db-config.json` file accordingly.

You can now initilize the database by running from the root of the project :

``` sh
exec/flup init
```

## Usage

### Set up a simulation

You can set up a simulation by providing a json file. Check the example files to see how it's done.

``` sh
exec/flup new path/to/myfile.json
```

You can specify a name for your simulation to find it more easily.

``` sh
exec/flup new path/to/myfile.json -n "My simulation"
```
