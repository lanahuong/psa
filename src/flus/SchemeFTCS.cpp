#include "SchemeFTCS.h"

SchemeFTCS::SchemeFTCS(const arma::cx_mat &phi_0, const arma::mat &pot,
                       double dx, double dy, double dt)
    : Solver(phi_0, pot, dx, dy, dt) {}

void SchemeFTCS::step() {
  /** i^2 = -1 */
  arma::cx_double i = arma::cx_double(0, 1);

  // Compute phitdt
  arma::cx_mat xpart =
      -arma::datum::h_bar * arma::datum::h_bar /
      (2 * arma::datum::m_e * dx * dx) *
      (phit.submat(2, 1, size(phitdt)) + phit.submat(0, 1, size(phitdt)));
  arma::cx_mat ypart =
      -arma::datum::h_bar * arma::datum::h_bar /
      (2 * arma::datum::m_e * dy * dy) *
      (phit.submat(1, 2, size(phitdt)) + phit.submat(1, 0, size(phitdt)));
  arma::cx_mat centerpart =
      (mat_pot % phit) +
      (i * arma::datum::h_bar / dt +
       arma::datum::h_bar * arma::datum::h_bar / (arma::datum::m_e * dx * dx) +
       arma::datum::h_bar * arma::datum::h_bar / (arma::datum::m_e * dy * dy)) *
          phit;

  phitdt = dt / (i * arma::datum::h_bar) *
           (xpart + ypart + centerpart.submat(1, 1, size(phitdt)));

  // Copy phitdt to phit for the next iteration with correct 0 padding
  phit.submat(1, 1, size(phitdt)) = phitdt;
}
