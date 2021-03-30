#include <gtest/gtest.h>

#include <armadillo>

#include "../src/SchemeFTCS.h"

TEST(SchemeFTCS, initialize) {
  arma::cx_mat phi;
  phi.ones(2, 2);
  SchemeFTCS solver = SchemeFTCS(phi, phi, 1, 1, 1);
  arma::cx_double zero = arma::cx_double(0, 0);

  ASSERT_TRUE(arma::size(solver.V) == arma::size(4, 4));
  ASSERT_EQ(solver.V(0, 0), zero);
  ASSERT_EQ(solver.V(0, 1), zero);
  ASSERT_EQ(solver.V(0, 2), zero);
  ASSERT_EQ(solver.V(0, 3), zero);
  ASSERT_EQ(solver.V(3, 0), zero);
  ASSERT_EQ(solver.V(3, 1), zero);
  ASSERT_EQ(solver.V(3, 2), zero);
  ASSERT_EQ(solver.V(3, 3), zero);
  ASSERT_EQ(solver.V(1, 0), zero);
  ASSERT_EQ(solver.V(2, 0), zero);
  ASSERT_EQ(solver.V(1, 3), zero);
  ASSERT_EQ(solver.V(2, 3), zero);

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
