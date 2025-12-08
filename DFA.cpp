//
// Created by wdapp on 11/17/2025.
//

#include "DFA.h"

/*
    function: GameDFA constructor
    inputs: strings a and b, and an alphabet set
    pre-condition: strings are valid and alphabet is non-empty
    outputs: none
    relationships: calls build_automaton()
    description: Initializes model strings and alphabet, then builds full DFA.
*/
GameDFA::GameDFA(const std::string &a, const std::string &b, const std::unordered_set<char> &alphabet) {
    strA = a;
    strB = b;
    this->alphabet = alphabet;
    build_automaton();
}


/*
    function: build_automaton
    inputs: none
    pre-condition: strA, strB, and alphabet are initialized
    outputs: none
    relationships: calls validate_strings, initial_build, add_absorbing_states, handle_fail_cases
    description: Constructs the full DFA, including initialization, absorbing states,
                 and fallback transitions.
    pseudo-code:
        1. Allocate a new DFA.
        2. Validate that strings do not share first character.
        3. Build initial prefix states and direct transitions.
        4. Mark full matches as absorbing.
        5. Fill missing transitions via fallback logic.
*/
void GameDFA::build_automaton() {
    DFA* inDFA = new DFA();
    builtDFA = inDFA;

    if (validate_strings()) {
        throw std::runtime_error("ERR: Equivalent Prefixing - strings cannot share a common first character");
    }

    initial_build();
    add_absorbing_states();
    handle_fail_cases();
}


/*
    function: validate_strings
    inputs: none
    pre-condition: strA and strB are initialized
    outputs: boolean (true if invalid)
    relationships: used by build_automaton
    description: Checks that the first characters of both strings differ.
*/
bool GameDFA::validate_strings() const {
    return (this->has_common_prefix());
}


/*
    function: handle_fail_cases
    inputs: none
    pre-condition: initial transitions exist for direct prefix expansions
    outputs: none
    relationships: adds transitions to builtDFA
    description: Fills all missing DFA transitions by computing fallback targets.
                 Uses optimized prefix-shifting and substring-view matching.
    pseudo-code:
        For each state and each alphabet symbol:
            1. If transition exists, continue.
            2. Build candidate fallback prefix = suffix(prefix) + char.
            3. Slide the starting index to test shorter suffixes.
            4. If a known prefix matches, add transition.
            5. Else fallback to empty prefix "".
*/
void GameDFA::handle_fail_cases() {

    for (auto &state : builtDFA->states) {

        if (state.absorbing) continue;

        const std::string &label = state.label;
        size_t L = label.size();

        for (char c : alphabet) {

            if (builtDFA->delta(state.id, c) != -1)
                continue;  // transition already exists

            // Construct initial fallback candidate
            std::string prefixCutter;
            prefixCutter.reserve(L + 1);

            if (L >= 1)
                prefixCutter = label.substr(1);
            else
                prefixCutter.clear();

            prefixCutter.push_back(c);

            // Slide the start index forward to look for another state
            size_t start = 0;

            while (start <= prefixCutter.size()) {

                std::string_view view(prefixCutter.c_str() + start,
                                      prefixCutter.size() - start);

                // Check substring view against existing states
                auto it = builtDFA->state_lookup.find(std::string(view));
                if (it != builtDFA->state_lookup.end()) {
                    builtDFA->add_transition(label, c, it->first);
                    break;
                }
                start++;
            }

            // If no prefix found, fallback to "" (root)
            if (start > prefixCutter.size()) {
                builtDFA->add_transition(label, c, "");
            }
        }
    }
}


/*
    function: has_common_prefix
    inputs: none
    pre-condition: strA and strB are initialized
    outputs: boolean
    relationships: used in validate_strings
    description: Returns true if strA and strB share their first character.
*/
bool GameDFA::has_common_prefix() const {
    return (strA[0] == strB[0]);
}


/*
    function: initial_build
    inputs: none
    pre-condition: state_lookup and builtDFA are initialized
    outputs: none
    relationships: calls inputs_to_set, buildInitStates, buildTransitionsFromStates
    description: Creates all prefix states and direct prefix transitions.
    pseudo-code:
        1. Gather all prefixes of strA and strB.
        2. Add them as states.
        3. Add direct prefix-extension transitions for both strings.
*/
void GameDFA::initial_build() {
    std::vector<std::string> inputs = {strA, strB};
    auto initStates = inputs_to_set(inputs);
    buildInitStates(initStates);
    buildTransitionsFromStates();
}


/*
    function: inputs_to_set
    inputs: vector<string> of patterns
    pre-condition: patterns are valid
    outputs: unordered_set<string> containing all prefixes
    relationships: used by initial_build
    description: Builds a prefix set including the empty string.
*/
std::unordered_set<std::string> GameDFA::inputs_to_set(const std::vector<std::string>& inputs) {
    auto out = std::unordered_set<std::string>();
    out.insert("");
    std::string stringIn;
    for (auto i : inputs) {
        stringIn = "";
        for (char j : i) {
            stringIn.push_back(j);
            out.insert(stringIn);
        }
    }
    return out;
}


/*
    function: buildInitStates
    inputs: a set of prefix strings
    pre-condition: builtDFA is allocated
    outputs: none
    relationships: used by initial_build
    description: Adds all prefix strings as states in the DFA.
*/
void GameDFA::buildInitStates(const std::unordered_set<std::string>& inputStrings) {
    for (const std::string& i : inputStrings) {
        builtDFA->add_state(i);
    }
}


/*
    function: buildTransitionsFromStates
    inputs: none
    pre-condition: all prefix states exist in builtDFA
    outputs: none
    relationships: used by initial_build
    description: Builds direct transitions for prefix extension within each pattern.
                 This handles the "normal" transitions before fallback is applied.
    pseudo-code:
        For each pattern:
            From full prefix to next shorter prefix, create transitions that extend the prefix.
*/
void GameDFA::buildTransitionsFromStates() {
    for (const std::string& i : {strA, strB}) {

        std::string fromString = i;
        std::string toString = i;
        toString.push_back(' '); // temporary padding

        do {
            fromString.pop_back();
            toString.pop_back();
            builtDFA->add_transition(fromString, toString.back(), toString);
        }
        while (!fromString.empty());
    }
}


/*
    function: add_absorbing_states
    inputs: none
    pre-condition: all states exist
    outputs: none
    relationships: modifies state list
    description: Marks the full-pattern states as absorbing, ending the game.
*/
void GameDFA::add_absorbing_states() const {
    for (auto i : {strA, strB}) {
        builtDFA->states.at(builtDFA->state_lookup.at(i)).absorbing = true;
    }
}
