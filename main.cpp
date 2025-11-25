//
// Created by wdapp on 11/17/2025.
//
#include <iostream>
#include "DFA.h"
#include "RecurrenceEquationBuilder.h"
#include "RecurrenceEquationSolver.h"
#include <bitset>

std::unordered_set<char> setUpAlphabet(const std::string& stringA, const std::string& stringB, int alphabetSize);

int main() {

    /*
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

    std::unordered_set<char> alphabet = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'X', 'Y', 'Z'};


    std::cout << "\n==== TEST DFA and RecurrenceEquationBuilder Together ====\n";
    GameDFA DFA = GameDFA(A, B, Alpha);
    DFA.printDFA();
    RecurrenceEquationBuilder builder0(&DFA);
    std::cout << "For 'DFA', the probability of A winning with " << DFA.getStringA() << " is " << getProbabilityOfWin(DFA.getStartState(), builder0);

//    GameDFA game("00", "12", {'0', '1', '2'});
    GameDFA game("00", "11", {'0', '1'});
    RecurrenceEquationBuilder builder(&game);
    std::cout << "\nFor 'game', the probability of A winning with " << game.getStringA() << " is " << getProbabilityOfWin(game.getStartState(), builder);

    std::cout << std::endl;
    testLinearSystemSolver();
    */




    std::string stringA;
    std::cout << "Enter a string for player A: ";
    std::cin >> stringA;

    std::string stringB;
    std::cout << "Enter a string for player B: ";
    std::cin >> stringB;


    int alphabetSize;
    std::cout << "Enter the size of the alphabet: ";
    std::cin >> alphabetSize;

    //validate the alphabet size
    while (true) {

        if (alphabetSize < stringA.size() + stringB.size()) {
            std::cout << "The alphabet size must be greater or equal to " << stringA.size() + stringB.size() << std::endl;
        }
        else if (alphabetSize > 256) {
            std::cout << "The alphabet size must be less than 256\n" << std::endl;
        }
        else {
            break;
        }

        std::cout << "Reenter the size of the alphabet: ";
        std::cin >> alphabetSize;

    }

    std::unordered_set<char> alphabet = setUpAlphabet(stringA, stringB, alphabetSize);

    GameDFA test1 = GameDFA(stringA, stringB, alphabet);
    RecurrenceEquationBuilder builder1(&test1);
    std::cout << "\nFor 'test1', the probability of A winning with " << test1.getStringA() << " is " << getProbabilityOfWin(test1.getStartState(), builder1) << std::endl;

    //test1.printDFA();

}




std::unordered_set<char> setUpAlphabet(const std::string& stringA, const std::string& stringB, int alphabetSize) {

    std::unordered_set<char> alphabet;
    int numberOfAlphabetCharsNeeded = alphabetSize;


    //put the unique chars in the strings into the alphabet
    for(auto i : stringA+stringB) {
        if (alphabet.count(i) == 0) {
            alphabet.insert(i);
            numberOfAlphabetCharsNeeded--;
        }
    }
    //std::cout << "After adding a and b strings to alpha, need this many more chars in alphabet: " << numberOfAlphabetCharsNeeded << std::endl;

    //fill out the rest of the alphabet with chars (it doesn't matter what they are)
    char alphaCharToAdd = 'NUL';
    while (numberOfAlphabetCharsNeeded != 0) {

        //if that char is not already in the alphabet, add it
        if (alphabet.count(alphaCharToAdd) == 0) {
            //std::cout << "added char " << alphaCharToAdd << std::endl;
            alphabet.insert(alphaCharToAdd);
            numberOfAlphabetCharsNeeded--;
        }

        alphaCharToAdd++;
    }

    return alphabet;
}
