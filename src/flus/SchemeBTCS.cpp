#include "SchemeBTCS.h"

SchemeBTCS::SchemeBTCS(const arma::cx_mat &phi_0, const arma::mat &pot,
                       double dx, double dy, double dt)
    : Solver(phi_0, pot, dx, dy, dt) {}

void SchemeBTCS::step() {
  const auto pot = mat_pot_;
  const auto f_t = phit_;
  arma::cx_mat g_np1 = f_t, g_n;

  const cx h2m = h_bar_ * h_bar_ / m_e_;
  const arma::cx_mat fact = 1 / (cx(0, h_bar_ / dt_) - h2m * (1 / (dx_ * dx_) + 1 / (dy_ * dy_)) - pot);

  do {
    g_n = std::move(g_np1);
    arma::cx_mat gn_pm_dx = shift_mat(g_n, 0, -1) + shift_mat(g_n, 0, 1);
    arma::cx_mat gn_pm_dy = shift_mat(g_n, -1, 0) + shift_mat(g_n, 1, 0);
    arma::cx_mat res = -(h2m / (2 * dx_ * dx_)) * gn_pm_dx -
                       (h2m / (2 * dy_ * dy_)) * gn_pm_dy +
                       cx(0, h_bar_ / dt_) * f_t;
    g_np1 = res % fact;
  } while (arma::norm(g_n - g_np1, "inf") > epsilon);

    phit_ = g_np1;
    phitdt_ = phit_.submat(1, 1, size(phitdt_));
}
