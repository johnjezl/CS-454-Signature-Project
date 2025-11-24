//
// Created by Student on 11/22/25.
//
#include "RecurrenceEquationBuilder.h"
#include "Rational.h"

Rational getProbabilityOfWin(int start_state, RecurrenceEquationBuilder builder);

std::vector<Rational> solveLinearSystem(const std::vector<std::vector<Rational>>& A, const std::vector<Rational>& b);

void testLinearSystemSolver();