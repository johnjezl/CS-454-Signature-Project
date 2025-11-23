//
// Created by wdapp on 11/17/2025.
//
#pragma once
#include <string>
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include <unordered_map>
#include <optional>
#include <iostream>

class DFA {
public:
    struct State {
        int id;
        std::string label;
        bool absorbing = false;
    };

    std::vector<State> states;
    std::vector<std::unordered_map<char, int>> transitions;
    std::unordered_map<std::string, int> state_lookup;

    int delta(int state_id, char input) {
        auto& row = transitions[state_id];
        auto it = row.find(input);

        if (it == row.end()) {
            // transition is missing
            return -1;       // or any sentinel value you prefer
        }

        return it->second;   // valid transition
    }

    std::optional<std::string> goTo(std::string state, char input){
        const int target = delta(state_lookup[state], input);
        switch (target) {
            case -1:
                return std::nullopt;
            default:
                return states.at(target).label;
        }
    }

    int add_state(const std::string& label,
                  const bool& absorbing = false){
        int id = states.size();
        states.push_back({ id, label, absorbing });
        transitions.push_back({});
        state_lookup[label] = id;
        return id;
    }

    void add_transition(const std::string& from, const char& input, const std::string& to){
        int from_id = state_lookup.at(from);
        int to_id   = state_lookup.at(to);
        transitions[from_id][input] = to_id;
    }
    void printDFA() const {
        std::cout << "==== DFA STRUCTURE ====\n";

        // Print all states
        std::cout << "States:\n";
        for (const auto& st : states) {
            std::cout << "  ID " << st.id
                      << " : \"" << st.label << "\""
                      << (st.absorbing ? "  [absorbing]" : "")
                      << "\n";
        }

        std::cout << "\nTransitions:\n";

        // Print transitions for each state
        for (size_t s = 0; s < transitions.size(); s++) {
            const auto& st = states[s];
            std::cout << "From state " << st.id
                      << " (\"" << st.label << "\"):\n";

            if (transitions[s].empty()) {
                std::cout << "    (no transitions)\n";
                continue;
            }

            for (const auto& [symbol, target] : transitions[s]) {
                const auto& tstate = states[target];
                std::cout << "    '" << symbol
                          << "'  ->  ID " << target
                          << " (\"" << tstate.label << "\")";
                if (tstate.absorbing) std::cout << "  [absorbing]";
                std::cout << "\n";
            }
            std::cout << "\n";
        }

        std::cout << "========================\n";
    }

};


class GameDFA {
public:
    GameDFA(const std::string& a,
            const std::string& b,
            const std::unordered_set<char>& alphabet);

    void build_automaton();
    void printDFA(){
        builtDFA->printDFA();
    }
    DFA* getDFA() const {
        return builtDFA;
    }

    std::string getStringA() const {
        return strA;
    }

    std::string getStringB() const {
        return strB;
    }
    std::unordered_set<char> getAlphabet() const {
        return alphabet;
    }

    int getStartState() const {
        return builtDFA->state_lookup.at("");
    }

private:
    std::string strA;
    std::string strB;
    std::unordered_set<char> alphabet;
    DFA* builtDFA;
    // internal helpers
    void initial_build();
    void buildInitStates(const std::unordered_set<std::string>& inputStrings);
    void buildTransitionsFromStates();
    void handle_fail_cases();      // Adjust internal strings if needed
    bool validate_strings() const;    // Check for common prefixes
    void add_absorbing_states() const;
    static std::unordered_set<std::string> inputs_to_set(const std::vector<std::string>& inputs);

    bool has_common_prefix() const;
};
