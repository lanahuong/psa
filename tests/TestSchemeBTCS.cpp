#include "../src/flus/SchemeBTCS.h"
#include <ctime>
#include <gtest/gtest.h>

constexpr int size = 50;
constexpr double step = 1;


TEST(Solver_BTCS, NormConsistency) {
  arma::arma_rng::set_seed((0));
  arma::cx_mat phi0(size, size);
  phi0.randn();
  phi0 *= (1 / std::sqrt(arma::accu(phi0 % arma::conj(phi0)).real() * 0.2 * 0.2));
  arma::mat V(size, size);
  V.zeros();
  SchemeBTCS slv(phi0, V, step, step, step/1000);

  double prev_norm = slv.phitdt_norm();

  for (int i = 0; i < 1; i++) {
    slv.step();
    double norm = slv.phitdt_norm();
      std::cout << "norm " << norm << "prev " << prev_norm << std::endl;
    ASSERT_TRUE(std::abs(norm - prev_norm) < epsilon);
    prev_norm = norm;
  }
}

TEST(Solver_BTCS, Dirac) {
  arma::cx_mat phi0(100, 100);
  phi0.randn();
  // phi0.at(0, 0) = 30;
  arma::mat V(100, 100, arma::fill::zeros);
  SchemeBTCS slv(phi0, V, step, step, 10e-6);

  //slv.normalize_phitdt();
  // std::cout << "inorm" << phitdt_norm(slv) << std::endl;
  for (int i = 0; i < 2; i++) {
    //    std::cout << "Norm " << phitdt_norm(slv) <<
    //    slv.phit << std::endl;
  //  slv.step();
    // std::cout << "inorm" << phitdt_norm(slv) << std::endl;
  }
}
