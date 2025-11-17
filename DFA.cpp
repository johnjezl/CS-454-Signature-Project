//
// Created by wdapp on 11/17/2025.
//

#include "DFA.h"

DFA GameDFA::build_automaton() {
    DFA inDFA = DFA();

}

bool GameDFA::validate_strings() const {
    return (this->has_common_prefix());
}

void GameDFA::handle_overlap_cases() {

}

GameDFA::GameDFA(const std::string &a, const std::string &b, const std::unordered_set<char> &alphabet) {

}

bool GameDFA::has_common_prefix() const {
    return (strA[0] == strB[0]);
}


std::vector<std::string> GameDFA::breakup_string(std::string inStr) {
    std::vector<std::string> out(inStr.length());
    std::string addStrings;
    for(int i = 0; i<inStr.length(); i++){
        addStrings.push_back(inStr.at(i));
        out.at(i) = addStrings;
    }
    return out;
}
