#include "../src/SchemeBTCS.h"
#include <ctime>
#include <gtest/gtest.h>

constexpr int size = 50;
constexpr double step = 10e-15;

double phitdt_norm(SchemeBTCS &slv) {
  return arma::accu(slv.phitdt % arma::conj(slv.phitdt)).real() * slv.dx *
         slv.dy;
}

TEST(Solver_BTCS, NormConsistency) {
  arma::arma_rng::set_seed((0));
  arma::cx_mat phi0(size, size);
  phi0.randn();
  arma::mat V(size, size);
  V.randn();
  SchemeBTCS slv(phi0, V, step, step, step);

  double prev_norm = phitdt_norm(slv);

  for (int i = 0; i < 1; i++) {
    slv.step();
    double norm = phitdt_norm(slv);
    //  std::cout << "norm " << norm << "prev " << prev_norm << std::endl;
    ASSERT_TRUE(std::abs(norm - prev_norm) < epsilon);
    prev_norm = norm;
  }
}

TEST(Solver_BTCS, Dirac) {
  arma::cx_mat phi0(100, 100);
  phi0.randn();
  // phi0.at(0, 0) = 30;
  arma::mat V(100, 100, arma::fill::zeros);
  SchemeBTCS slv(phi0, V, 10e-2, 10e-2, 10e-10);

  slv.normalize_phitdt();
  // std::cout << "inorm" << phitdt_norm(slv) << std::endl;
  for (int i = 0; i < 2; i++) {
    //    std::cout << "Norm " << phitdt_norm(slv) <<
    //    slv.phit << std::endl;
    slv.step();
    // std::cout << "inorm" << phitdt_norm(slv) << std::endl;
  }
}
