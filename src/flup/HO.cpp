#include "HO.h"
#include <iomanip> // hermit << operator formatting

/**
 * @file HO.cpp
 */



/**
 * Computes and evaluates hermit polynomial
 * 
 * This function uses the recursion relation definition of the hermit polynomials to fill the values matrix up to rank n polynomials with the object's mesh
 * 
 * @param n rank of the desired polynomial
 */
void Hermit::hermit(int n) {
    if (n < 0) {
        throw "Invalid polynomial index. Positive integers only.";
    }
    int m = hermit_values.n_cols;
    while (m <= n) {
        arma::colvec next_col;
        if (m == 1)
            next_col = 2*mesh;
        else
            next_col = 2*(mesh%hermit_values.col(m-1)) - 2*(m-1)*hermit_values.col(m-2);
        hermit_values.insert_cols(m, next_col);
        m++;
    }
}
    

/**
 * Accessor for n-th hermit polynomial
 * 
 * @param n rank of the desired polynomial
 * @return Returns evaluation of n-th polynomial for the object's mesh
 */
arma::colvec Hermit::get(int n) {
    if ((int) hermit_values.n_cols <= n) {
        throw "Invalid polynomial index. Must be inferior or equal to max given.";
    }
    return hermit_values.col(n);
}

/**
 * << operator to print hermit object on stdout stream
 *
 * Prints out the hermit_values matrix, showing all computed ranks of polynomials, along the object's mesh. Use Hermit::insert_size to set matrix cells size.
 * @param stream stream on which to add hermit print
 * @param hermit object to print
 * @return stream with hermit_values matrix printed on it
 */
std::ostream& operator<<(std::ostream &stream, const Hermit &hermit) {
    for (int z = 0; z < (int) hermit.hermit_values.n_cols; ++z) {
        for (int n = 0; n < (int) hermit.hermit_values.n_rows; ++n) {
            stream /*<< std::setfill('.')*/ << std::setw(hermit.insert_size) << hermit.hermit_values(n, z) << " ";
        }
        stream << std::endl;
    }
    return stream;
}



/**
 * Computes the factorial
 * 
 * This recursive function computes the factorial of n
 * 
 * @param n integer to computes the factorial of
 * @return the factorial of n
 */
int Schrodinger::factorial(int n){
    if (n==0 || n==1) return 1;
    return n * factorial(n-1);
}


/**
 * Vectorized solution of the Schrodinger equation
 *
 * @param n energy level
 * @param z mesh on which to evaluate the solution
 * @return the solution computed along given mesh
 */
arma::colvec Schrodinger::psi(int n, arma::colvec z) {
    Hermit hermit(n , sqrt(m*w/h_bar) * z);
    return (1 / (sqrt(pow(2, n) * Schrodinger::factorial(n)))) * pow((m*w)/(pi*h_bar), 0.25) * arma::exp(-m*w*(z%z) / (2*h_bar)) % hermit.get(n);
}