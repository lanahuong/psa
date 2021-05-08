#include "SchemeBTCS.h"

SchemeBTCS::SchemeBTCS(const arma::cx_mat &phi_0, const arma::mat &pot,
                       double dx, double dy, double dt)
        : Solver(phi_0, pot, dx, dy, dt) {}

void SchemeBTCS::step() {
    arma::cx_mat g_np1 = phit_, g_n;

    const double h_bar = h_bar_ / step_scale_;

    const cx hmdx = h_bar * h_bar / (m_e_ * dx_ * dx_ * step_scale_ * step_scale_);
    const cx hmdy = h_bar * h_bar / (m_e_ * dy_ * dy_ * step_scale_ * step_scale_);
    const arma::cx_mat fact = -(1. / h_bar) * mat_pot_(1, 1, size(phitdt_)) - hmdx - hmdy;

    do {
        g_n = g_np1;
        arma::cx_mat gn_pm_dx = g_n(2, 1, size(phitdt_)) + g_n(0, 1, size(phitdt_));
        arma::cx_mat gn_pm_dy = g_n(1, 2, size(phitdt_)) + g_n(1, 0, size(phitdt_));
        arma::cx_mat res = (hmdx / 2.) * gn_pm_dx + (hmdy / 2.) * gn_pm_dy + fact % g_n(1, 1, size(phitdt_));
        g_np1(1, 1, size(phitdt_)) = res * cx(0, dt_ * step_scale_) + phit_(1, 1, size(phitdt_));
    } while (arma::norm(g_n - g_np1, "inf") > epsilon);

    phit_ = g_np1;
    phitdt_ = phit_(1, 1, size(phitdt_));
}
