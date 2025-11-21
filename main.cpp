//
// Created by wdapp on 11/17/2025.
//
#include <iostream>
#include "DFA.h"
int main() {
    std::cout << "Hello World!";
    std::string A, B;
    std::cout << "\nenter A: ";
    std::cin >> A;
    std::cout << "\nenter B: ";
    std::cin >> B;
    std::unordered_set<char> Alpha;
    for(auto i : A+B){
        Alpha.insert(i);
    }
    GameDFA DFA = GameDFA(A, B, Alpha);
        DFA.printDFA();
}