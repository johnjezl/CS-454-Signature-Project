// Test Suite for CS 454 Signature Project

#include <iostream>
#include <iomanip>
#include <cassert>
#include "DFA.h"
#include "RecurrenceEquationBuilder.h"
#include "RecurrenceEquationSolver.h"
#include <set>

void test_failed(const std::string& msg) {
    std::cout << "FAILED: " << msg << "\n";
    exit(1);
}

// Helper to print equation system
void print_equation_system(const RecurrenceEquationBuilder& builder) {
    auto coeffs = builder.get_coefficients();
    auto consts = builder.get_constants();
    int numEquations = builder.get_num_equations();

    std::cout << "\n  Num Equations: " << numEquations << std::endl;
    std::cout << "  Coefficients: " << coeffs.size() << std::endl;
    std::cout << "  Sparsity: " << std::fixed << std::setprecision(2)
              << (100.0 * coeffs.size() / (numEquations * numEquations)) << "%\n";
}

void test_basic_construction() {
    std::cout << "Create Basic DFA and Equation Builder" << std::endl;

    GameDFA game("00", "11", {'0', '1'});
    RecurrenceEquationBuilder builder(&game);

    assert(builder.get_num_equations() > 0);
    assert(builder.get_coefficients().size() > 0);
    assert(builder.get_constants().size() == static_cast<size_t>(builder.get_num_equations()));

}

void test_alphabet_of_len(int len, int expected_states) {
    std::cout << "Test Alphabet (Length = " << len << ")" << std::endl;
    std::unordered_set<char> alphabet = {};
    for (int i = 0; i < len; i++) {
        alphabet.insert('0'+i);
    }

    char b2 = ('0' + static_cast<char>(alphabet.size()));
    std::string strA = "00";
    std::string strB = std::string("1") + b2;

    GameDFA game(strA, strB, alphabet);
    RecurrenceEquationBuilder builder(&game);

    assert(builder.get_num_equations() == expected_states);
}

void test_boundary_conditions() {
    std::cout << "Test Boundary Conditions" << std::endl;

    GameDFA game("00", "12", {'0', '1', '2'});
    RecurrenceEquationBuilder builder(&game);

    auto consts = builder.get_constants();
    auto coeffs = builder.get_coefficients();

    // Count boundary conditions
    int num_ones = 0;  // Winning states (b = 1)
    int num_zeros = 0; // Losing states (b = 0)

    for (const Rational& c : consts) {
        if (c == Rational(1)) num_ones++;
        if (c == Rational(0)) num_zeros++;
    }

    // Should have exactly 1 winning state
    assert(num_ones == 1);

    // Verify winning state has identity equation
    bool found_identity = false;
    for (size_t i = 0; i < consts.size(); i++) {
        if (consts[i] == Rational(1)) {
            // Check that row i has 1*P[i] = 1
            for (const auto& [row, col, val] : coeffs) {
                if (row == static_cast<int>(i) && col == static_cast<int>(i) && val == Rational(1)) {
                    found_identity = true;
                }
            }
        }
    }
    assert(found_identity);
}

void test_coefficient_structure() {
    std::cout << "Test Coefficient Structure" << std::endl;

    GameDFA game("00", "11", {'0', '1'});
    RecurrenceEquationBuilder builder(&game);

    auto coeffs = builder.get_coefficients();
    auto consts = builder.get_constants();
    int n = builder.get_num_equations();

    // Check that all row indices are valid
    for (const auto& [row, col, val] : coeffs) {
        assert(row >= 0 && row < n);
        assert(col >= 0 && col < n);
    }

    // Check that non-terminal states have diagonal = 1
    for (int i = 0; i < n; i++) {
        // Skip terminal states (constant = 1 for win, 0 for loss with identity row)
        if (consts[i] == Rational(1) || consts[i] == Rational(0)) {
            // Check if this is a terminal state by looking for identity row
            bool is_terminal = false;
            for (const auto& [row, col, val] : coeffs) {
                if (row == i && col == i && val == Rational(1)) {
                    // Count non-zero coefficients in this row
                    int row_count = 0;
                    for (const auto& [r, c, v] : coeffs) {
                        if (r == i) row_count++;
                    }
                    if (row_count == 1) is_terminal = true;
                }
            }
            if (is_terminal) continue;
        }

        // Find diagonal element for non-terminal
        bool found_diagonal = false;
        for (const auto& [row, col, val] : coeffs) {
            if (row == i && col == i) {
                assert(val == Rational(1));
                found_diagonal = true;
            }
        }

        if (!found_diagonal) {
            test_failed("Missing diagonal for non-terminal state");
        }
    }
}

void test_sparsity() {
    std::cout << "Test Matrix Sparsity" << std::endl;

    GameDFA game("00", "12", {'0', '1', '2'});
    RecurrenceEquationBuilder builder(&game);

    auto coeffs = builder.get_coefficients();
    int n = builder.get_num_equations();

    double sparsity = 100.0 * coeffs.size() / (n * n);

    // Sparse representation should have < 100% filled
    assert(sparsity < 100.0);

    // For typical DFAs, should be quite sparse
    std::cout << " (sparsity: " << std::fixed << std::setprecision(1)
              << sparsity << "%)";
}

void test_DFA_RecurrenceEquationBuilder_integration() {
    std::cout << "Integration: DFA to RecurrenceEquationBuilder" << std::endl;

    // Build DFA
    GameDFA game("00", "11", {'0', '1'});

    // Build equations
    RecurrenceEquationBuilder builder(&game);

    assert(builder.get_num_equations() > 0);
    assert(builder.get_coefficients().size() > 0);

    print_equation_system(builder);
}

void test_assignment_example() {
    std::cout << "Test Assignment Example (x='00', y='12')" << std::endl;

    std::cout << "\n  Expected: P(A wins) = 3/7\n";

    GameDFA game("00", "12", {'0', '1', '2'});
    RecurrenceEquationBuilder builder(&game);

    Rational result = getProbabilityOfWin(game.getStartState(), builder);
    std::cout << "  Actual: P(A wins) = " << result << "\n";

    assert(result == Rational(3, 7));
    std::cout << "  PASSED!\n";
}

void test_symmetric_case() {
    std::cout << "Test Symmetric Case (x='00', y='11', alphabet={0,1})" << std::endl;

    std::cout << "\n  Expected: P(A wins) = 1/2\n";

    GameDFA game("00", "11", {'0', '1'});
    RecurrenceEquationBuilder builder(&game);

    Rational result = getProbabilityOfWin(game.getStartState(), builder);
    std::cout << "  Actual: P(A wins) = " << result << "\n";

    assert(result == Rational(1, 2));
    std::cout << "  PASSED!\n";
}

void test_output_interface() {
    std::cout << "Test Output Interface to Solver" << std::endl;

    GameDFA game("00", "11", {'0', '1'});
    RecurrenceEquationBuilder builder(&game);

    // Verify all output methods work
    auto coeffs = builder.get_coefficients();
    auto consts = builder.get_constants();
    int n = builder.get_num_equations();
    auto mapping = builder.get_state_mapping();

    // Check types and sizes
    assert(coeffs.size() > 0);
    assert(consts.size() == static_cast<size_t>(n));
    assert(mapping.size() == static_cast<size_t>(n));

    // Verify COO format structure (row, col, val)
    for (const auto& [row, col, val] : coeffs) {
        assert(row >= 0 && row < n);
        assert(col >= 0 && col < n);
    }
}

void test_different_string_lengths() {
    std::cout << "Test Different String Lengths" << std::endl;

    // Shorter first string
    GameDFA game1("0", "111", {'0', '1'});
    RecurrenceEquationBuilder builder1(&game1);
    assert(builder1.get_num_equations() > 0);

    // Longer first string
    GameDFA game2("000", "11", {'0', '1'});
    RecurrenceEquationBuilder builder2(&game2);
    assert(builder2.get_num_equations() > 0);
}

void test_larger_alphabet() {
    std::cout << "Test Larger Alphabet" << std::endl;

    GameDFA game("ab", "cd", {'a', 'b', 'c', 'd'});
    RecurrenceEquationBuilder builder(&game);

    assert(builder.get_num_equations() > 0);
}


int main() {
    try {
        std::cout << "\n=== RecurrenceEquationBuilder Tests ===\n";
        test_basic_construction();
        std::cout << " PASSED\n";

        test_boundary_conditions();
        std::cout << " PASSED\n";

        test_coefficient_structure();
        std::cout << " PASSED\n";

        test_sparsity();
        std::cout << " PASSED\n";

        std::cout << "\n=== Integration Tests ===\n";
        test_DFA_RecurrenceEquationBuilder_integration();

        test_assignment_example();

        test_symmetric_case();

        test_output_interface();
        std::cout << " PASSED\n";

        std::cout << "\n=== Edge Case Tests ===\n";
        test_different_string_lengths();
        std::cout << " PASSED\n";

        test_larger_alphabet();
        std::cout << " PASSED\n";

        std::cout << "\n=== ALL TESTS PASSED ===\n";
        return 0;

    } catch (const std::exception& e) {
        std::cout << "\nFAILED!\n";
        std::cout << "Exception: " << e.what() << "\n";
        return 1;
    }
}
