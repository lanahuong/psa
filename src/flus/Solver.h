/**
 * @file Solver.h
 */

#ifndef PROJET_PSA_SOLVER_H
#define PROJET_PSA_SOLVER_H

#include <armadillo>

constexpr double epsilon = 10e-10;
using cx = std::complex<double>;

/**
 * @class Solver
 * This class is an interface for our Solver. The function step() must be
 * implemented in a derived class.
 */
class Solver {
protected:
    double scale_ = 10e-10; //angstrom
    double h_bar_ = arma::datum::h_bar * (scale_ * scale_);
    double m_e_ = arma::datum::m_e * (scale_ * scale_);


public:
    /**
     * Everytime the step() function is called, this state (at t) is updated and
     * contains the new state (at t+dt) of the field
     */
    arma::cx_mat phit_;
    arma::cx_mat phitdt_;
    arma::mat mat_pot_;
    const double dx_, dy_, dt_;

    /**
     * Constructor to be used
     * @param phi_0 the initial state of the field
     * @param pot the potential
     * @param dx step on the x axis
     * @param dy step on the y axis
     * @param dt time step
     */
    Solver(const arma::cx_mat &phi_0, const arma::mat &pot, double dx, double dy,
           double dt);

    /**
     * Moves forward the internal state by dt.
     */
    virtual void step() = 0;

    /**
     * Shifts the matrix by a certain amount of rows and columns (can be negative)
     * @param rows
     * @param cols
     * @return
     */
    static arma::cx_mat shift_mat(arma::cx_mat, int rows, int cols);

    void normalize_phitdt();

    arma::cx_mat get_state();
};

#endif // PROJET_PSA_SOLVER_H
