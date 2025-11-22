#pragma once
#include "DFA.h"
#include <vector>
#include <map>
#include <tuple>
#include <cmath>

class RecurrenceEquationBuilder {
private:
    GameDFA* game_dfa;
    DFA* dfa;
    int alphabet_size;
    std::unordered_set<char> alphabet;
    std::string strA;
    std::string strB;
    
    int num_states;
    std::map<int, int> state_to_index;
    std::vector<std::tuple<int, int, double>> coefficients;
    std::vector<double> constants;
    
    void initialize_system();
    void generate_recurrences();
    void generate_equation_for_state(int state_id);
    void apply_boundary_conditions();
    
public:
    RecurrenceEquationBuilder(GameDFA* game);
    
    void build_system();
    
    std::vector<std::tuple<int, int, double>> get_coefficients() const {
        return coefficients;
    }
    
    std::vector<double> get_constants() const {
        return constants;
    }
    
    int get_num_equations() const {
        return num_states;
    }
    
    std::map<int, int> get_state_mapping() const {
        return state_to_index;
    }
};
