//
// Created by wdapp on 11/17/2025.
//
#include <iostream>
#include "DFA.h"
#include "RecurrenceEquationBuilder.h"
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


    std::cout << "\n==== TEST DFA and RecurrenceEquationBuilder Together ====\n";
    GameDFA DFA = GameDFA(A, B, Alpha);
    DFA.printDFA();

//    GameDFA game("00", "12", {'0', '1', '2'});
    GameDFA game("00", "11", {'0', '1'});
    RecurrenceEquationBuilder builder(&game);

   }
