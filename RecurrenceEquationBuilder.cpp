#include "RecurrenceEquationBuilder.h"
#include "Rational.h"
#include <map>

/*
    constructor: RecurrenceEquationBuilder
    inputs: game - pointer to the GameDFA instance
    pre-condition: GameDFA class is defined and provides access to DFA, alphabet, and player strings
    outputs: none
    relationships: initializes class members and calls build_system to set up the recurrence equations
    description: Constructs a RecurrenceEquationBuilder object by initializing its members from the provided GameDFA and building the system of equations.
    pseudo-code:
        1. Set game_dfa to the provided game pointer.
        2. Retrieve and set dfa, alphabet, alphabet_size, strA, and strB from game_dfa.
        3. Call build_system() to generate the recurrence equations.
*/
RecurrenceEquationBuilder::RecurrenceEquationBuilder(GameDFA* game)
    : game_dfa(game)
{
    // Get references from GameDFA
    dfa = game_dfa->getDFA();
    alphabet = game_dfa->getAlphabet();
    alphabet_size = alphabet.size();
    strA = game_dfa->getStringA();
    strB = game_dfa->getStringB();

    build_system();
}


/*
    function: initialize_system
    inputs: none
    pre-condition: class members 'dfa', 'state_to_index', 'constants' are defined
    outputs: none
    relationships: none
    description: Initializes the system by setting up state mappings and resizing constants.
    pseudo-code:
        1. Set num_states to the number of states in the DFA.
        2. For each state in the DFA, map its ID to an equation index.
        3. Resize the constants vector to hold num_states elements, initialized to zero.
*/
void RecurrenceEquationBuilder::initialize_system() {
    num_states = dfa->states.size();

    // Create state-to-equation-index mapping
    for (int i = 0; i < num_states; i++) {
        state_to_index[dfa->states[i].id] = i;
    }

    // Initialize constants
    constants.resize(num_states, Rational(0));
}


/*
    function: generate_recurrences
    inputs: none
    pre-condition: class members 'dfa', 'num_states' are defined
    outputs: none
    relationships: calls generate_equation_for_state for each non-absorbing state
    description: Generates recurrence equations for all non-absorbing states in the DFA.
    pseudo-code:
        1. For each state_id from 0 to num_states - 1:
            a. If the state is absorbing, skip it.
            b. Otherwise, call generate_equation_for_state(state_id).
*/
void RecurrenceEquationBuilder::generate_recurrences() {
    for (int state_id = 0; state_id < num_states; state_id++) {
	    // Skip absorbing states
        if (dfa->states[state_id].absorbing) {
            continue;
        }

        generate_equation_for_state(state_id);
    }
}


/*
    function: generate_equation_for_state
    inputs: state_id - the ID of the state for which to generate the equation
    pre-condition: class members 'dfa', 'alphabet', 'alphabet_size', 'state_to_index', 'coefficients', 'constants' are defined
    outputs: none
    relationships: uses Rational arithmetic and updates coefficients and constants
    description: Generates the recurrence equation for a given non-absorbing state based on its transitions.
    pseudo-code:
        1. Determine the equation row index for the given state_id.
        2. Initialize a map to accumulate coefficients, starting with P(i) coefficient as 1.
        3. Compute the inverse of the alphabet size as a Rational number (1/M).
        4. For each symbol in the alphabet:
            a. Get the next state via the DFA transition function.
            b. If the transition is missing, throw an error.
            c. Determine the column index for the next state.
            d. Subtract (1/M) from the coefficient for P(next_state).
        5. For each non-zero coefficient in the accumulator, add it to the coefficients list.
        6. Set the constant for this equation row to 0.
*/
void RecurrenceEquationBuilder::generate_equation_for_state(int state_id) {
    int row = state_to_index[state_id];

    // Build equation: P(i) = 0.5 × P(δ(i,0)) + 0.5 × P(δ(i,1))
    std::map<int, Rational> col_accumulator;
    col_accumulator[row] = Rational(1);  // Coefficient for P(i)

    // this is just 1/M
    Rational invM(1, (long long)alphabet_size);

    // For each symbol in alphabet, add transition
    for (char symbol : alphabet) {
        int next_state = dfa->delta(state_id, symbol);

	// Should not occur
        if (next_state == -1) {
            throw std::runtime_error("DFA missing transition (state " + std::to_string(state_id) +
                ", symbol '" + std::string(1, symbol) + "')");	//the (1, symbol) converts symbol to a string first
        }

        int next_col = state_to_index[next_state];

        // Below is same as col_accumulator[next_col] -= 1/M
        col_accumulator[next_col] = col_accumulator[next_col] - invM;
    }

    // Only add non-zero coefficients (sparse representation)
    for (const auto& [col, coeff] : col_accumulator) {
        if (!isZero(coeff)) {
            coefficients.push_back({row, col, coeff});
        }
    }

    constants[row] = Rational(0);
}



/*
    function: apply_boundary_conditions
    inputs: none
    pre-condition: class members 'dfa', 'num_states', 'state_to_index', 'coefficients', 'constants', 'strA', 'strB' are defined
    outputs: none
    relationships: updates coefficients and constants for absorbing states
    description: Applies boundary conditions for absorbing states in the DFA, setting their probabilities based on whether they are winning or losing states.
    pseudo-code:
        1. For each state_id from 0 to num_states - 1:
            a. If the state is absorbing:
                i. Determine the equation row index for the state.
                ii. Add the equation P(state) = constant to the coefficients.
                iii. If the state's label matches strA, set the constant to 1 (A wins).
                iv. If the state's label matches strB, set the constant to 0 (B wins).
                v. If the state's label is unknown, throw an error.
*/
void RecurrenceEquationBuilder::apply_boundary_conditions() {
    for (int state_id = 0; state_id < num_states; state_id++) {
        const DFA::State& state = dfa->states[state_id];

        if (state.absorbing) {
            int row = state_to_index[state_id];

            // Terminal state equation: P(state) = constant
            coefficients.push_back({row, row, Rational(1)});

	        // Check if this is winning state (strA) or losing state (strB)
            if (state.label == strA) {
                constants[row] = Rational(1);  // A wins
            } else if (state.label == strB) {
                constants[row] = Rational(0);  // B wins
            } else {
                throw std::runtime_error("Unknown terminal state: " + state.label);
            }
        }
    }
}

/*
    function: build_system
    inputs: none
    pre-condition: class members are defined
    outputs: none
    relationships: calls initialize_system, generate_recurrences, and apply_boundary_conditions
    description: Builds the entire system of recurrence equations by initializing, generating recurrences, and applying boundary conditions.
    pseudo-code:
        1. Call initialize_system().
        2. Call generate_recurrences().
        3. Call apply_boundary_conditions().
*/
void RecurrenceEquationBuilder::build_system() {
    initialize_system();
    generate_recurrences();
    apply_boundary_conditions();
}
