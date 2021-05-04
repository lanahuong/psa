#include <gtest/gtest.h>

#include <armadillo>


#define protected public

#include "../src/flus/SchemeFTCS.h"


TEST(SchemeFTCS, initialize) {
  arma::cx_mat phi;
  phi.ones(2, 2);
  arma::mat V;
  V.ones(2, 2);
  SchemeFTCS solver = SchemeFTCS(phi, V, 1, 1, 1);
  arma::cx_double zero = arma::cx_double(0, 0);

  ASSERT_TRUE(arma::size(solver.mat_pot_) == arma::size(4, 4));
  ASSERT_EQ(solver.mat_pot_(0, 0), zero);
  ASSERT_EQ(solver.mat_pot_(0, 1), zero);
  ASSERT_EQ(solver.mat_pot_(0, 2), zero);
  ASSERT_EQ(solver.mat_pot_(0, 3), zero);
  ASSERT_EQ(solver.mat_pot_(3, 0), zero);
  ASSERT_EQ(solver.mat_pot_(3, 1), zero);
  ASSERT_EQ(solver.mat_pot_(3, 2), zero);
  ASSERT_EQ(solver.mat_pot_(3, 3), zero);
  ASSERT_EQ(solver.mat_pot_(1, 0), zero);
  ASSERT_EQ(solver.mat_pot_(2, 0), zero);
  ASSERT_EQ(solver.mat_pot_(1, 3), zero);
  ASSERT_EQ(solver.mat_pot_(2, 3), zero);

  ASSERT_TRUE(arma::size(solver.phit_) == arma::size(4, 4));
  ASSERT_EQ(solver.phit_(0, 0), zero);
  ASSERT_EQ(solver.phit_(0, 1), zero);
  ASSERT_EQ(solver.phit_(0, 2), zero);
  ASSERT_EQ(solver.phit_(0, 3), zero);
  ASSERT_EQ(solver.phit_(3, 0), zero);
  ASSERT_EQ(solver.phit_(3, 1), zero);
  ASSERT_EQ(solver.phit_(3, 2), zero);
  ASSERT_EQ(solver.phit_(3, 3), zero);
  ASSERT_EQ(solver.phit_(1, 0), zero);
  ASSERT_EQ(solver.phit_(2, 0), zero);
  ASSERT_EQ(solver.phit_(1, 3), zero);
  ASSERT_EQ(solver.phit_(2, 3), zero);

  ASSERT_TRUE(arma::size(solver.get_phitdt()) == arma::size(2, 2));
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
  double step = 1e-5;
  arma::arma_rng::set_seed((0));
  arma::cx_mat phi0(size, size);
  phi0.randn();
  phi0 = 10e-10*(phi0 + cx(0,1) * (phi0));
  arma::mat V(size, size);
  V.ones(); // Building a square of ones;
  V.submat(1, 1, arma::size(size-2, size-2)) = arma::mat(size -2, size-2, arma::fill::zeros);
  //std::cout << V;

  SchemeFTCS solver(phi0, V, step, step, step * 10e-5);
 // ASSERT_TRUE(std::abs(solver.phitdt_norm() - 1) < epsilon);

  double prev_norm = solver.phitdt_norm();

  for (int i = 0; i < 10; i++) {
    solver.step();
    double norm = solver.phitdt_norm();
    std::cout << norm - prev_norm << std::endl;
    ASSERT_TRUE(std::abs(norm - prev_norm) < epsilon);
    prev_norm = norm;
  }
}
