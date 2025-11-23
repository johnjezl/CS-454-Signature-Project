//
// Created by Student on 11/22/25.
//
#include "RecurrenceEquationSolver.h"

//todo change these later to use precise numbers


//outputs the solution to the equation for the start state (probability of A winning)
double getProbabilityOfWin(int start_state, RecurrenceEquationBuilder builder) {

    //make a matrix of the coefficients and initially fill all spots with 0.
    //the size is nxn where n is the number of equations
    int matrixSize = builder.get_num_equations();
    std::vector<std::vector<double>> matrix(matrixSize, std::vector<double>(matrixSize, 0));

    //fill in the values for each coefficient in their proper spots
    for (const auto& currentTuple : builder.get_coefficients()) {
        matrix[get<0>(currentTuple)][get<1>(currentTuple)] = get<2>(currentTuple);
    }

    //make the vector of constants
    std::vector<double> constants = builder.get_constants();

    //do the solving
    std::vector<double> solutions = solveLinearSystem(matrix, constants);

    //output the solution gotten from the equation corresponding to start state
    return solutions[start_state];

}



//This function was written by ChatGPT
// ---------- General Solver: A * x = b ----------
// A: n x n matrix of doubles
// b: n-dimensional vector of doubles
// returns x: n-dimensional vector of doubles
std::vector<double> solveLinearSystem( std::vector<std::vector<double>> A, std::vector<double> b) {

    int n = (int)A.size();

    if (n == 0 || (int)A[0].size() != n || (int)b.size() != n) {
        throw std::runtime_error("Matrix A must be n x n and b must be size n");
    }

    // Build augmented matrix [A | b] (Sort of combines matrix A and b)
    std::vector<std::vector<double>> aug(n, std::vector<double>(n + 1));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            aug[i][j] = A[i][j];
        aug[i][n] = b[i];
    }

    // Gauss-Jordan elimination with partial pivoting
    for (int col = 0; col < n; ++col) {
        // Find a pivot row (non-zero in this column) at or below 'col'
        int pivotRow = col;
        while (pivotRow < n && aug[pivotRow][col] == 0)
            ++pivotRow;

        if (pivotRow == n)
            throw std::runtime_error("System has no unique solution (zero column)");

        // Swap pivot row into place
        if (pivotRow != col)
            std::swap(aug[pivotRow], aug[col]);

        double pivot = aug[col][col];

        // Normalize pivot row
        for (int j = col; j <= n; ++j)
            aug[col][j] = aug[col][j] / pivot;

        // Eliminate this column in all other rows
        for (int row = 0; row < n; ++row) {
            if (row == col) continue;
            double factor = aug[row][col];
            if (factor != 0) {
                for (int j = col; j <= n; ++j) {
                    aug[row][j] = aug[row][j] - factor * aug[col][j];
                }
            }
        }
    }

    // Now aug = [I | x]
    std::vector<double> x(n);
    for (int i = 0; i < n; ++i)
        x[i] = aug[i][n];

    return x;
}



