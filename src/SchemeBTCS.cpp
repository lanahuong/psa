#include "SchemeBTCS.h"

SchemeBTCS::SchemeBTCS(const arma::cx_mat &phi_0, const arma::mat &pot,
                       double dx, double dy, double dt)
    : Solver(phi_0, pot, dx, dy, dt) {}

void SchemeBTCS::step() {
  const auto pot = mat_pot;
  const auto f_t = phit;
  arma::cx_mat g_np1 = f_t, g_n;

  const cx h2m = reduced_planck * reduced_planck / neutron_mass;
  const arma::cx_mat fact = 1 / (cx(0, reduced_planck / dt) -
                                 h2m * (1 / (dx * dx) + 1 / (dy * dy)) - pot);

  do {
    g_n = std::move(g_np1);
    arma::cx_mat gn_pm_dx = shift_mat(g_n, 0, -1) + shift_mat(g_n, 0, 1);
    arma::cx_mat gn_pm_dy = shift_mat(g_n, -1, 0) + shift_mat(g_n, 1, 0);
    arma::cx_mat res = -(h2m / (2 * dx * dx)) * gn_pm_dx -
                       (h2m / (2 * dy * dy)) * gn_pm_dy +
                       cx(0, reduced_planck / dt) * f_t;
    g_np1 = res % fact;
  } while (arma::norm(g_n - g_np1, "inf") > epsilon);

  phit = g_np1;
  phitdt = phit.submat(1, 1, size(phitdt));
}
