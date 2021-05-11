#include "SchemeBTCS.h"

SchemeBTCS::SchemeBTCS(const arma::cx_mat &phi_0, const arma::mat &pot,
                       double dx, double dy, double dt)
        : Solver(phi_0, pot, dx, dy, dt) {}

void SchemeBTCS::step() {
    arma::cx_mat g_np1 = phit_, g_n;

    const double h_bar = h_bar_;
    const cx psi_fac = cx(0, 1) * h_bar * dt_ / (2 * m_e_);
    const cx pot_fac = -cx(0, 1) * dt_ / h_bar;
    const arma::cx_mat pot = pot_fac * mat_pot_(1, 1, size(phitdt_));
    do {
        g_n = g_np1;
        arma::cx_mat gn_pm_dx = g_n(2, 1, size(phitdt_)) + g_n(0, 1, size(phitdt_)) - 2* g_n(1, 1, size(phitdt_));
        arma::cx_mat gn_pm_dy = g_n(1, 2, size(phitdt_)) + g_n(1, 0, size(phitdt_)) - 2* g_n(1, 1, size(phitdt_));
        g_np1(1, 1, size(phitdt_)) = phitdt_ + (psi_fac / (dx_ * dx_)) * gn_pm_dx + (psi_fac / (dy_ * dy_)) * gn_pm_dy + pot;
    } while (arma::norm(g_n(1, 1, size(phitdt_)) - g_np1(1, 1, size(phitdt_)), "inf") > epsilon);
    phit_ = g_np1;
    phitdt_ = g_np1(1, 1, size(phitdt_));
}
