#ifndef PROJET_PSA_SOLVER_CTCS_H
#define PROJET_PSA_SOLVER_CTCS_H

#include <armadillo>

constexpr double epsilon = 10e-10;
constexpr double reduced_planck = 6.582119514 * 10e-22;
constexpr double neutron_mass = 1.674927471 * 10e-27;

using cx = std::complex<double>;

struct cross_cx {
    cx x_m_dx, x_p_dx, y_m_dy, y_p_dy;
};


class solver_ctcs {
private:
    const arma::mat mat_pot;
    const double dx, dy, dt;

    cx F(double pot, cx psi, cross_cx t, cross_cx t_p_dt) const;

public:
    solver_ctcs(arma::mat pot, double dx, double dy, double dt);
     arma::cx_mat step_ctcs(arma::cx_mat phi_0);
};


#endif //PROJET_PSA_SOLVER_CTCS_H
