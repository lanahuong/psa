#ifndef PROJET_PSA_SOLVER_H
#define PROJET_PSA_SOLVER_H

#include <armadillo>

constexpr double epsilon = 10e-10;
constexpr double reduced_planck = 6.582119514*10e-22;
constexpr double neutron_mass = 1.674927471*10e-27;

using cx = std::complex<double>;

class solver {
protected:
    const arma::mat mat_pot;
    const double dx, dy, dt;
public:
    arma::cx_mat internal_state;
    solver(arma::cx_mat phi_0, arma::mat pot, double dx, double dy, double dt);
    virtual void step() = 0;

    double compute_internal_norm();
};


#endif //PROJET_PSA_SOLVER_H
