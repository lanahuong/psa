#ifndef __SCHEME_FTCS_H
#define __SCHEME_FTCS_H

#include "Solver.h"
#include <armadillo>

class SchemeFTCS : public Solver {
public:
  /**
   * @brief Constructor
   */
  SchemeFTCS(const arma::cx_mat &phi_0, const arma::mat &pot, double dx,
             double dy, double dt);

  /**
   * @brief Perform one time iteration of the FTCS scheme
   */
  void step() override;
};

#endif // __SCHEME_FTCS_H
