/**
 * @file solver.h
 */

#ifndef PROJET_PSA_SOLVER_H
#define PROJET_PSA_SOLVER_H

#include <armadillo>

constexpr double epsilon = 10e-10;
constexpr double reduced_planck = 6.582119514 * 10e-22;
constexpr double neutron_mass = 1.674927471 * 10e-27;
using cx = std::complex<double>;

/**
 * @class solver
 * This class is an interface for our solver. The function step() must be implemented in a derived class.
 */
class solver {
protected:
    const arma::mat mat_pot;
    const double dx, dy, dt;

public:
    /**
     * Everytime the step() function is called, this state (at t) is updated and contains the new state (at t+dt) of the field
     */
    arma::cx_mat internal_state;

    /**
     * Constructor to be used
     * @param phi_0 the initial state of the field
     * @param pot the potential
     * @param dx step on the x axis
     * @param dy step on the y axis
     * @param dt time step
     */
    solver(arma::cx_mat phi_0, arma::mat pot, double dx, double dy, double dt);

    /**
     * Moves forward the internal state by dt.
     */
    virtual void step() = 0;

    /**
     * Integrates the psi state on the whole surface/domain
     * @return
     */
    double compute_internal_norm() const;

    /**
     * Add a border composed of zeroes around the matrix
     * @return
     */
    static arma::cx_mat add_zero_border(arma::cx_mat);

    static arma::mat add_zero_border(arma::mat);

    /**
     * Remove the border of the matrix
     * @return
     */
    static arma::cx_mat shed_zero_border(arma::cx_mat);

    /**
     * Shifts the matrix by a certain amount of rows and columns (can be negative)
     * @param rows
     * @param cols
     * @return
     */
    static arma::cx_mat shift_mat(arma::cx_mat, int rows, int cols);

    void normalize_internal_state();

};


#endif //PROJET_PSA_SOLVER_H
