#include "SchemeFTCS.h"

SchemeFTCS::SchemeFTCS(const arma::cx_mat &phi_0, const arma::mat &pot,
                       double dx, double dy, double dt)
        : Solver(phi_0, pot, dx, dy, dt) {}

void SchemeFTCS::step() {
    /** i^2 = -1 */
    cx i(0, 1);
    // Compute phitdt
    auto s = arma::size(phit_.n_rows - 4, phit_.n_cols - 4);
    arma::cx_mat xpart = -h_bar_ * h_bar_ / (2 * m_e_ * dx_ * dx_) *
                         (phit_.submat(2, 1, size(phitdt_)) + phit_.submat(0, 1, size(phitdt_)));
    arma::cx_mat ypart = -h_bar_ * h_bar_ / (2 * m_e_ * dy_ * dy_) *
                         (phit_.submat(1, 2, size(phitdt_)) + phit_.submat(1, 0, size(phitdt_)));
    arma::cx_mat centerpart = (mat_pot_ % phit_) + ((i * h_bar_ / dt_) + (h_bar_ * h_bar_ / (m_e_ * dx_ * dx_)) + (h_bar_ * h_bar_ / (m_e_ * dy_ * dy_))) * phit_;
    arma::cx_mat tmp = centerpart.submat(1, 1, size(phitdt_));
    tmp(1, 1, s) += xpart(1, 1, s) + ypart(1, 1, s);

    phitdt_ = tmp * (dt_ / (i * h_bar_));
    // Copy phitdt to phit for the next iteration with correct 0 padding
    phit_.submat(1, 1, size(phitdt_)) = phitdt_;
}
