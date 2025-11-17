//
// Created by wdapp on 11/17/2025.
//
#pragma once
#include <string>
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include <unordered_map>

class DFA {
public:
    struct State {
        int id;
        bool is_accepting = false;
    };

    // Basis for the transition table, each item in the vector is a different transition
    std::vector<std::unordered_map<char, int>> transitions;
    int start_state = 0;

    void add_state(bool accepting = false) {
        transitions.push_back({});
    }

    void add_transition(int from, char symbol, int to) {
        transitions[from][symbol] = to;
    }
};

class GameDFA {
public:
    GameDFA(const std::string& a,
            const std::string& b,
            const std::unordered_set<char>& alphabet);

    DFA build_automaton();
    bool validate_strings() const;    // Check for common prefixes
    void handle_overlap_cases();      // Adjust internal strings if needed

private:
    std::string strA;
    std::string strB;
    std::unordered_set<char> alphabet;

    // internal helpers
    std::vector<std::string> breakup_string(std::string inStr);
    bool has_common_prefix() const;
};
