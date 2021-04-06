#ifndef PROJET_PSA_SCHEMEBTCS_H
#define PROJET_PSA_SCHEMEBTCS_H

#include "Solver.h"

class SchemeBTCS : public Solver {
public:
  SchemeBTCS(const arma::cx_mat &phi_0, const arma::mat &pot, double dx,
             double dy, double dt);

  void step() override;
};

#endif // PROJET_PSA_SCHEMEBTCS_H
