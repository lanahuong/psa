#ifndef PROJET_PSA_SOLVER_CTCS_H
#define PROJET_PSA_SOLVER_CTCS_H

#include "solver.h"

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
