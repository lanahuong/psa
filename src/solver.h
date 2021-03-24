#ifndef PROJET_PSA_SOLVER_H
#define PROJET_PSA_SOLVER_H

#include <armadillo>

constexpr double epsilon = 10e-10;
constexpr double reduced_planck = 6.582119514 * 10e-22;
constexpr double neutron_mass = 1.674927471 * 10e-27;

enum class solver_type{
    FTCS, BTCS, CTCS
};

class solver {

};

#endif //PROJET_PSA_SOLVER_H
