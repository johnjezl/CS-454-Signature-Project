//
// Created by wdapp on 11/17/2025.
//

#include "DFA.h"

GameDFA::GameDFA(const std::string &a, const std::string &b, const std::unordered_set<char> &alphabet) {
    strA = a;
    strB = b;
    this->alphabet = alphabet;
    build_automaton();
}

DFA* GameDFA::build_automaton() {
    DFA* inDFA = new DFA();
    builtDFA = inDFA;
    validate_strings();
    initial_build();
    handle_fail_cases();
    return builtDFA;
}

bool GameDFA::validate_strings() const {
    return (!this->has_common_prefix());
}

void GameDFA::handle_fail_cases() {
    std::string prefixCutter = "";
    for(auto i : builtDFA->states){
        for(auto j : alphabet){
            if(builtDFA->delta(i.id, j) == -1){
                prefixCutter = i.label.substr(1) + j;
                while(builtDFA->state_lookup.find(prefixCutter) == builtDFA->state_lookup.end() && prefixCutter != ""){
                    prefixCutter.erase(0,1);
                }
                builtDFA->add_transition(i.label, j, prefixCutter);
            }
        }
    }
}

bool GameDFA::has_common_prefix() const {
    return (strA[0] == strB[0]);
}





void GameDFA::initial_build() {
    std::vector<std::string> inputs = {strA, strB};
    auto initStates = inputs_to_set(inputs);
    buildInitStates(initStates);
    buildTransitionsFromStates();
}

std::unordered_set<std::string> GameDFA::inputs_to_set(const std::vector<std::string>& inputs) {
    auto out = std::unordered_set<std::string>();
    out.insert("");
    std::string stringIn;
    for(auto i : inputs){
        stringIn = "";
        for(char j : i){
            stringIn.push_back(j);
            out.insert(stringIn);
        }
    }
    return out;
}

void GameDFA::buildInitStates(const std::unordered_set<std::string>& inputStrings) {
    for(const std::string& i : inputStrings){
        builtDFA->add_state(i);
    }
}

void GameDFA::buildTransitionsFromStates() {
    for(const std::string& i : {strA, strB}){
        std::string fromString = i;
        std::string toString = i;
        toString.push_back(' ');
        do{
            fromString.pop_back();
            toString.pop_back();
            builtDFA->add_transition(fromString, toString.back() ,toString);
        }
        while(!fromString.empty());
    }
}
