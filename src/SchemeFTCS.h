#ifndef __SCHEME_FTCS_H
#define __SCHEME_FTCS_H

#include <armadillo>

class SchemeFTCS {
public:
  /** phi(t) wave function at time t */
  arma::cx_mat phit;
  /** phi(t+dt) wave function at time t+dt */
  arma::cx_mat phitdt;
  /** V(x,y) the potential field */
  arma::cx_mat V;
  /** dt the time pace */
  double dt;
  /** dx the x pace */
  double dx;
  /** dy the y pace */
  double dy;
  /** i^2 = -1 */
  arma::cx_double i = arma::cx_double(0, 1);

  /**
   * @brief Constructor
   */
  SchemeFTCS(arma::cx_mat phit, arma::cx_mat V, double dt, double dx,
             double dy);

  /**
   * @brief Perform one time iteration of the FTCS scheme
   */
  void step();
};

#endif // __SCHEME_FTCS_H
