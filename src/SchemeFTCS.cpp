#include "SchemeFTCS.h"

SchemeFTCS::SchemeFTCS(arma::cx_mat phit, arma::cx_mat V, double dt, double dx,
                       double dy) {
  // Size of matrix with padding of 1
  int nx = phit.n_rows + 2;
  int ny = phit.n_cols + 2;

  // Create phit with 0 padding
  this->phit.zeros(nx, ny);
  this->phit.submat(1, 1, size(phit)) = phit;

  // Create phitdt withoutpadding
  this->phitdt.zeros(nx - 2, ny - 2);

  // Create V with 0 padding
  this->V.zeros(nx, ny);
  this->V.submat(1, 1, size(V)) = V;

  this->dt = dt;
  this->dx = dx;
  this->dy = dy;
}

void SchemeFTCS::step() {
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
      arma::dot(V, phit) +
      (i * arma::datum::h_bar / dt +
       arma::datum::h_bar * arma::datum::h_bar / arma::datum::m_e * dx * dx +
       arma::datum::h_bar * arma::datum::h_bar / arma::datum::m_e * dy * dy) *
          phit;

  phitdt = dt / (i * arma::datum::h_bar) *
           (xpart + ypart + centerpart.submat(1, 1, size(phitdt)));

  // Copy phitdt to phit for the next iteration with correct 0 padding
  phit.submat(1, 1, size(phitdt)) = phitdt;
}
