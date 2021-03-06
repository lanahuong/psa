/**
 * @file Solver.h
 */

#ifndef PROJET_PSA_SOLVER_H
#define PROJET_PSA_SOLVER_H

#include <armadillo>

constexpr double epsilon = 1e-10;
using cx = std::complex<double>;

/**
 * @class Solver
 * This class is an interface for our Solver. The function step() must be
 * implemented in a derived class.
 */
class Solver {
protected:
    double phi_norm_ = 1; // The value to get back the real phi as we normalise the phi matrix internally to deal with values close to 1.
    const double step_scale_ = 1e-15; // femto, depends ont the inputs, shouldn't be changed without changing the FRONTEND!!!!

    const double m_e_ = arma::datum::m_e;
    const double h_bar_ = arma::datum::h_bar;

    const double m_e_scale_ = 1;//arma::datum::m_e / m_e_; // UNUSED, but Deduced automatically
    const double h_bar_scale_ = 1;// arma::datum::h_bar / h_bar_; //UNUSED

    /**
     * Everytime the step() function is called, this state (at t) is updated and
     * contains the new state (at t+dt) of the field
     */
    arma::cx_mat phitdt_;
    arma::cx_mat phit_;
    arma::mat mat_pot_;
    const double dx_, dy_, dt_;

public:
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
     * Moves forward the internal state by n*dt.
     */
    void step_n(int n);

    /**
     * Shifts the matrix by a certain amount of rows and columns (can be negative)
     * @param rows
     * @param cols
     * @return
     */
    static arma::cx_mat shift_mat(arma::cx_mat, int rows, int cols);

    /**
     * Returns the current state of the solver
     * @return
     */
    arma::cx_mat get_phitdt() const;

    /**
     * Returns the psi % psi_bar matrix which is real
     * @return
     */
    arma::mat get_phitdt_absolute() const;

    double phitdt_norm() const;

    double get_step_scale() const;
};

#endif // PROJET_PSA_SOLVER_H
