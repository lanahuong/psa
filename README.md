# Advanced scientific programming project : 2D-FD solver for the time-dependent non-relativistic Schrödinger equation

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
- numpy (pip)
- pyevtk
- pymongo (pip)
- armadillo
- swig

## Set up

After installing the requirements, create a MongoDB database called `flu_db` and
a user of the database with the "readWrite" role called `flu` with password `flupwd`.

```db.createUser(
{	user: "flu",
	pwd: "flupwd",
	roles:[{role: "readWrite" , db:"flu_db"}]})
```

If you wish to use other names and password you will need to change the
`src/db-config.json` file accordingly.

You can now initialize the database by running from the root of the project :

``` sh
exec/flup init
```

In `binding/` generate the bindings with :

``` sh
make
```

## Usage

### Set up a simulation

You can set up a simulation by providing a json file. Check the example files to see how it's done in `examples/json`.

``` sh
exec/flup new path/to/myfile.json
```

You need give a unique name to your simulation.

If you are just playing with the initial state of your simulation you can run the above command as much as you want and
the simulation will simply be updated.

### Visualise a simulation

To see your simulation in Paraview, you need to generate the VTR files. Use the command :

```sh
exec/flup visual sim_name
```

This with generate VTR files for the potential field and all the frames computed using their module.

You can choose to display the real part with the option `-r`.

With the option `-f` you can choose the first frame to generate and with `-l` the last one.

Generate only the potential with `-p`.

Examples :

``` sh
# Generate VTR files for all frames of an harmonic osillator (real part)
exec/flup visual ho -r
# Generate VTR files for frames 66 and greater of a gaussian simulation
exec/flup visual gaus -f 66
# Generate VTR files for frames 50 to 250 of gaussian simulation
exec/flup visual gaus -f 50 -l 250
```

You can use this while the simulation is running to check results, only generate new frames with `-f`.

### List simulations

You can list the simulations in the database with both `flup` and `flus` :

``` sh
exec/flup list
exec/flus list
```

### Run a simulation

You can run the simulation with `flus start`. You need to specify the number of frames to compute with the option `-n` :

 ```sh 
exec/flus start sim_name -n 100
```

Other parameters can be customized :
- time pace in fs with `-dt`, default is 100 fs
- time supersampling with `-s`, default is 20
- the scheme used by the solver with `-m`, choose from `ctcs ftcs btcs`, default is ctcs
 
## Examples

You can try to run examples located in `examples/`.

Videos of simulations are in `video`, json files used to generate those are in `json` images for potential are in the `examples` directory.
