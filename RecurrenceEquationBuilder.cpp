#include "RecurrenceEquationBuilder.h"

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

void RecurrenceEquationBuilder::initialize_system() {
    num_states = dfa->states.size();
    
    // Create state-to-equation-index mapping
    for (int i = 0; i < num_states; i++) {
        state_to_index[dfa->states[i].id] = i;
    }
    
    // Initialize constants
    constants.resize(num_states, 0.0);
}

void RecurrenceEquationBuilder::generate_recurrences() {
    for (int state_id = 0; state_id < num_states; state_id++) {
        // Skip absorbing states
        if (dfa->states[state_id].absorbing) {
            continue;
        }
        
        generate_equation_for_state(state_id);
    }
}

void RecurrenceEquationBuilder::generate_equation_for_state(int state_id) {
    int row = state_to_index[state_id];
    
    // Build equation: P(i) = 0.5 × P(δ(i,0)) + 0.5 × P(δ(i,1))
    std::map<int, double> col_accumulator;
    col_accumulator[row] = 1.0;  // Coefficient for P(i)
    
    // For each symbol in alphabet, add transition
    for (char symbol : alphabet) {
        int next_state = dfa->delta(state_id, symbol);
        
        // Should not occur
        if (next_state == -1) {
            throw std::runtime_error("DFA missing transition (state " + std::to_string(state_id) +
                                     ", symbol '" + symbol + "')");
        }
        
        int next_col = state_to_index[next_state];
        col_accumulator[next_col] -= 1.0 / alphabet_size;
    }
    
    // Only add non-zero coefficients (sparse representation)
    for (const auto& [col, coeff] : col_accumulator) {
        if (std::abs(coeff) > 1e-10) {
            coefficients.push_back({row, col, coeff});
        }
    }
    
    constants[row] = 0.0;
}

void RecurrenceEquationBuilder::apply_boundary_conditions() {
    for (int state_id = 0; state_id < num_states; state_id++) {
        const DFA::State& state = dfa->states[state_id];
        
        if (state.absorbing) {
            int row = state_to_index[state_id];
            
            // Terminal state equation: P(state) = constant
            coefficients.push_back({row, row, 1.0});
            
            // Check if this is winning state (strA) or losing state (strB)
            if (state.label == strA) {
                constants[row] = 1.0;  // Player A wins
            } else if (state.label == strB) {
                constants[row] = 0.0;  // Player B wins
            } else {
                throw std::runtime_error("Unknown terminal state: " + state.label);
            }
        }
    }
}

void RecurrenceEquationBuilder::build_system() {
    initialize_system();
    generate_recurrences();
    apply_boundary_conditions();
}
