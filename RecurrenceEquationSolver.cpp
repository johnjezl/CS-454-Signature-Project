//
// Created by Student on 11/22/25.
//
#include "RecurrenceEquationSolver.h"


//outputs the solution to the equation for the start state (probability of A winning)
Rational getProbabilityOfWin(int start_state, RecurrenceEquationBuilder builder) {

    //make a matrix of the coefficients and initially fill all spots with 0.
    //the size is nxn where n is the number of equations
    int matrixSize = builder.get_num_equations();
    std::vector<std::vector<Rational>> matrix(matrixSize, std::vector<Rational>(matrixSize, 0));

    //fill in the values for each coefficient in their proper spots
    for (const auto& currentTuple : builder.get_coefficients()) {
        matrix[get<0>(currentTuple)][get<1>(currentTuple)] = get<2>(currentTuple);
    }

    //make the vector of constants
    std::vector<Rational> constants = builder.get_constants();

    //do the solving
    std::vector<Rational> solutions = solveLinearSystem(matrix, constants);

    //output the solution gotten from the equation corresponding to start state
    return solutions[start_state];

}



//This function was written by ChatGPT
// ---------- General Solver: A * x = b ----------
// A: n x n matrix of Rationals
// b: n-dimensional vector of Rationals
// returns x: n-dimensional vector of Rationals
std::vector<Rational> solveLinearSystem( const std::vector<std::vector<Rational>>& A, const std::vector<Rational>& b)
{

    int n = static_cast<int>(A.size());

    //quick size checks
    if (n == 0 || static_cast<int>(A[0].size()) != n || static_cast<int>(b.size()) != n) {
        throw std::runtime_error("Matrix A must be n x n and b must be size n");
    }

    /*
    //in depth size checks

    if (n == 0)
        throw std::runtime_error("Matrix A must be non-empty");

    if (static_cast<int>(b.size()) != n)
        throw std::runtime_error("Vector b must have size n");

    // Check all rows of A are length n
    for (int i = 0; i < n; ++i) {
        if (static_cast<int>(A[i].size()) != n)
            throw std::runtime_error("Matrix A must be n x n");
    }
    */

    // Build augmented [A | b]
    std::vector<std::vector<Rational>> aug(n, std::vector<Rational>(n + 1));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            aug[i][j] = A[i][j];
        aug[i][n] = b[i];
    }

    // Gauss–Jordan with exact arithmetic
    for (int col = 0; col < n; ++col) {

        // Find non-zero pivot in/under this row
        int pivotRow = col;
        while (pivotRow < n && isZero(aug[pivotRow][col]))
            ++pivotRow;

        if (pivotRow == n)
            throw std::runtime_error("System has no unique solution (zero column)");

        // Swap pivot row into place
        if (pivotRow != col)
            std::swap(aug[pivotRow], aug[col]);

        // Normalize pivot row
        Rational pivot = aug[col][col];
        for (int j = col; j <= n; ++j)
            aug[col][j] = aug[col][j] / pivot;

        // Eliminate this column in all other rows
        for (int row = 0; row < n; ++row) {
            if (row == col) continue;
            Rational factor = aug[row][col];
            if (!isZero(factor)) {
                for (int j = col; j <= n; ++j) {
                    aug[row][j] = aug[row][j] - factor * aug[col][j];
                }
            }
        }
    }

    // Now aug = [I | x]
    std::vector<Rational> x(n);
    for (int i = 0; i < n; ++i)
        x[i] = aug[i][n];

    return x;
}



//tests the solver with 4 systems of equations with known answers
void testLinearSystemSolver() {

    std::cout << "\n========Testing some systems of equations========\n";

    //case 1 (A=00, B=12)
    std::vector<std::vector<Rational>> A = {
        { Rational(2,3),  Rational(-1,3), Rational(0) },
        { Rational(-1,3), Rational(1),    Rational(-1,3) },
        { Rational(-1,3), Rational(-1,3), Rational(2,3) }
    };
    std::vector<Rational> b = {
        Rational(0),
        Rational(1,3),
        Rational(0)
    };
    std::vector<Rational> solutions = solveLinearSystem(A,b);
    std::cout << "Solution is " << solutions[2] << ". Should be 3/7" << std::endl;


    //case 2 (Just equations, not related to the game)
    A = {
        { Rational(-1),   Rational(1,2), Rational(1,2), Rational(0)   },
        { Rational(1,2),  Rational(-1),  Rational(0),   Rational(1,2) },
        { Rational(0),    Rational(0),    Rational(1),   Rational(-1,2) },
        { Rational(0),    Rational(0),    Rational(-1,2), Rational(1)  }
    };
    b = {
        Rational(0),
        Rational(0),
        Rational(0),
        Rational(1,2)
    };
    solutions = solveLinearSystem(A,b);
    std::cout << "Solution is " << solutions[0] << ". Should be 4/9" <<std::endl;


    //case 3 (Just unrelated equations)
    A = {
        { Rational(-2,3), Rational(1,6), Rational(1,2) },
        { Rational(0),    Rational(1,2), Rational(0)   },
        { Rational(0),    Rational(0),   Rational(1,6) }
    };
    b = {
        Rational(0),
        Rational(0),
        Rational(1,6)
    };
    solutions = solveLinearSystem(A,b);
    std::cout << "Solution is " << solutions[0] << ". Should be 3/4" <<std::endl;


    //case 4 (A = 00, B = 11)
    A = {
        { Rational(1),   Rational(0),    Rational(0),    Rational(0),    Rational(0) },
        { Rational(0),   Rational(1),    Rational(-1,2), Rational(-1,2), Rational(0) },
        { Rational(-1,2), Rational(-1,2), Rational(1),    Rational(0),    Rational(0) },
        { Rational(0),   Rational(0),    Rational(0),    Rational(1),    Rational(0) },
        { Rational(0),   Rational(-1,2), Rational(-1,2), Rational(0),    Rational(1) }
    };
    b = {
        Rational(0),
        Rational(0),
        Rational(0),
        Rational(1),
        Rational(0)
    };
    solutions = solveLinearSystem(A,b);
    std::cout << "Solution is " << solutions[4] << ". Should be 1/2" <<std::endl;


}
