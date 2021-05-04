#include "../src/flus/SchemeCTCS.h"
#include "../src/flus/SchemeBTCS.h"
#include <ctime>
#include <gtest/gtest.h>

constexpr int size = 20;
constexpr double step = 1e-5;


TEST(BTCSvCTCS, NormConsistency) {
    arma::arma_rng::set_seed((0));
    arma::cx_mat phi0(size, size);
    phi0.randn();
   // phi0 *= 1 / std::sqrt((arma::accu(phi0 % arma::conj(phi0)).real() * step * step));
   // phi0 *= 10e-12;
    arma::mat V(size, size);
    V.ones();
    SchemeCTCS ctcs(phi0, V, step, step, step);
    SchemeBTCS btcs(phi0, V, step, step, step);

//    ASSERT_TRUE(std::abs(phitdt_norm(ctcs) - 1) < epsilon);
//    ASSERT_TRUE(std::abs(phitdt_norm(btcs) - 1) < epsilon);

    for (int i = 0; i < 1; i++) {
        ctcs.step();
        btcs.step();
        auto norm = arma::norm(ctcs.get_phitdt() - btcs.get_phitdt());
        std::cout << norm << std::endl;
        ASSERT_TRUE(norm < epsilon);
    }
}
