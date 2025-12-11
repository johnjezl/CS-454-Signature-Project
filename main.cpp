/*
 *  Team: William Dappen, John Jezl, Angelica Rohrs
 *
 *  AI Disclosure
 *     AI tools were used to assist with:
 *      - Creating unit and integration tests
 *      - Developing the Rational type for exact arithmetic
 *      - Refining matrix math functions in the solver
 */
#include <iostream>
#include "DFA.h"
#include "RecurrenceEquationBuilder.h"
#include "RecurrenceEquationSolver.h"
#include <bitset>

std::unordered_set<char> setUpAlphabet(const std::string& stringA, const std::string& stringB, int alphabetSize);
void testDFAs ();
void testAllAlphabetSizes(std::string stringA, std::string stringB);

int main() {

    std::string stringA;
    std::cout << "Enter a string for player A: ";
    std::cin >> stringA;

    std::string stringB;
    std::cout << "Enter a string for player B: ";
    std::cin >> stringB;


    int alphabetSize;
    std::cout << "Enter the size of the alphabet (32 or less): ";
    std::cin >> alphabetSize;

    //calculate min alphabet size with given strings
    std::unordered_set<char> uniqueLetters;
    for (auto i : stringA+stringB) {
        if (uniqueLetters.count(i) == 0) {
            uniqueLetters.insert(i);
        }
    }
    int minSize = uniqueLetters.size();

    if (minSize > 32) {
        throw std::runtime_error("Cannot create an alphabet 32 or less with the given strings.");
    }

    //validate the alphabet size
    while (true) {

        if (alphabetSize < minSize) {
            std::cout << "The alphabet size must be greater or equal to " << minSize << std::endl;
        }
        else if (alphabetSize > 32) {
            std::cout << "The alphabet size must be less than or equal to 32\n" << std::endl;
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
    char alphaCharToAdd = '!';
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


