#include <gtest/gtest.h>

#include <armadillo>

#include "../src/flus/SchemeFTCS.h"

double phitdt_norm(SchemeFTCS &slv) {
  return arma::accu(slv.phitdt % arma::conj(slv.phitdt)).real() * slv.dx *
         slv.dy;
}

TEST(SchemeFTCS, initialize) {
  arma::cx_mat phi;
  phi.ones(2, 2);
  arma::mat V;
  V.ones(2, 2);
  SchemeFTCS solver = SchemeFTCS(phi, V, 1, 1, 1);
  arma::cx_double zero = arma::cx_double(0, 0);

  ASSERT_TRUE(arma::size(solver.mat_pot) == arma::size(4, 4));
  ASSERT_EQ(solver.mat_pot(0, 0), zero);
  ASSERT_EQ(solver.mat_pot(0, 1), zero);
  ASSERT_EQ(solver.mat_pot(0, 2), zero);
  ASSERT_EQ(solver.mat_pot(0, 3), zero);
  ASSERT_EQ(solver.mat_pot(3, 0), zero);
  ASSERT_EQ(solver.mat_pot(3, 1), zero);
  ASSERT_EQ(solver.mat_pot(3, 2), zero);
  ASSERT_EQ(solver.mat_pot(3, 3), zero);
  ASSERT_EQ(solver.mat_pot(1, 0), zero);
  ASSERT_EQ(solver.mat_pot(2, 0), zero);
  ASSERT_EQ(solver.mat_pot(1, 3), zero);
  ASSERT_EQ(solver.mat_pot(2, 3), zero);

  ASSERT_TRUE(arma::size(solver.phit) == arma::size(4, 4));
  ASSERT_EQ(solver.phit(0, 0), zero);
  ASSERT_EQ(solver.phit(0, 1), zero);
  ASSERT_EQ(solver.phit(0, 2), zero);
  ASSERT_EQ(solver.phit(0, 3), zero);
  ASSERT_EQ(solver.phit(3, 0), zero);
  ASSERT_EQ(solver.phit(3, 1), zero);
  ASSERT_EQ(solver.phit(3, 2), zero);
  ASSERT_EQ(solver.phit(3, 3), zero);
  ASSERT_EQ(solver.phit(1, 0), zero);
  ASSERT_EQ(solver.phit(2, 0), zero);
  ASSERT_EQ(solver.phit(1, 3), zero);
  ASSERT_EQ(solver.phit(2, 3), zero);

  ASSERT_TRUE(arma::size(solver.phitdt) == arma::size(2, 2));
}

/*
TEST(SchemeFTCS, step) {
  arma::cx_mat phi;
  phi.ones(2, 2);
  phi = phi / 4;
  SchemeFTCS solver = SchemeFTCS(phi, phi, 1, 1, 1);

  solver.step();
  arma::mat phi0mod = arma::abs(phi);
  arma::mat phi1mod = arma::abs(solver.phitdt);
}
*/

TEST(SolverFTCS, NormConsistency) {
  int size = 10;
  double step = 10e-15;
  arma::arma_rng::set_seed((0));
  arma::cx_mat phi0(size, size);
  phi0.ones();
  phi0 *=
      (1 / std::sqrt(arma::accu(phi0 % arma::conj(phi0)).real() * 0.2 * 0.2));
  arma::mat V(size, size);
  V.ones();
  SchemeFTCS solver(phi0, V, step, step, step);

  double prev_norm = phitdt_norm(solver);


  for (int i = 0; i < 10; i++) {
    solver.step();
    double norm = phitdt_norm(solver);
    std::cout << norm - prev_norm << std::endl;
    ASSERT_TRUE(std::abs(norm - prev_norm) < epsilon);
    prev_norm = norm;
  }
}
