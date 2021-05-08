#include "SchemeBTCS.h"

SchemeBTCS::SchemeBTCS(const arma::cx_mat &phi_0, const arma::mat &pot,
                       double dx, double dy, double dt)
        : Solver(phi_0, pot, dx, dy, dt) {}

void SchemeBTCS::step() {
    arma::cx_mat g_np1 = phit_, g_n;

    const cx psi_fac = cx(0, 1) * h_bar_ * dt_ / (2 * m_e_ * step_scale_ * step_scale_);
    const cx pot_fac = -cx(0, 1) * dt_ * step_scale_ * step_scale_ / h_bar_;
    const arma::cx_mat pot = pot_fac * mat_pot_(1, 1, size(phitdt_));
    auto s = arma::size(phit_.n_rows - 4, phit_.n_cols - 4);
    do {
        g_n = g_np1;
        arma::cx_mat gn_center = g_n(1, 1, size(phitdt_));
        arma::cx_mat gn_pm_dx = g_n(2, 1, size(phitdt_)) - gn_center + g_n(0, 1, size(phitdt_)) - gn_center;
        arma::cx_mat gn_pm_dy = g_n(1, 2, size(phitdt_)) - gn_center + g_n(1, 0, size(phitdt_)) - gn_center;
        arma::cx_mat res = phitdt_;// + (psi_fac / (dx_ * dx_)) * gn_pm_dx + (psi_fac / (dy_ * dy_)) * gn_pm_dy;

        res(1, 1, s) += (psi_fac / (dx_ * dx_)) * gn_pm_dx(1, 1, s);
        res(1, 1, s) += (psi_fac / (dy_ * dy_)) * gn_pm_dy(1, 1, s);
        // res += pot;

        std::cout << res << std::endl;
        std::cout << "phit" << arma::norm(phitdt_) << std::endl;
        std::cout << "gn_pm_dx" << arma::norm(gn_pm_dx) << std::endl;
        std::cout << "fact" << (psi_fac / (dx_ * dx_)) << std::endl;

        g_np1(1, 1, size(phitdt_)) = res;
    } while (arma::norm(g_n(1, 1, size(phitdt_)) - g_np1(1, 1, size(phitdt_)), "inf") > epsilon);
    phit_ = g_np1;
    phitdt_ = g_np1(1, 1, size(phitdt_));
}
