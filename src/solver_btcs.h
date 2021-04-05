#ifndef PROJET_PSA_SOLVER_BTCS_H
#define PROJET_PSA_SOLVER_BTCS_H

#include "solver.h"

class solver_btcs : public solver {
public:
    solver_btcs(const arma::cx_mat &phi_0, const arma::mat &pot, double dx, double dy, double dt);

    void step() override;
};


#endif //PROJET_PSA_SOLVER_BTCS_H
