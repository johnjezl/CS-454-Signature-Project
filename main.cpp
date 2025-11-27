//
// Created by wdapp on 11/17/2025.
//
#include <iostream>
#include "DFA.h"
#include "RecurrenceEquationBuilder.h"
#include "RecurrenceEquationSolver.h"
#include <bitset>

std::unordered_set<char> setUpAlphabet(const std::string& stringA, const std::string& stringB, int alphabetSize);
void testDFAs ();
void testAllAlphabetSizes(std::string stringA, std::string stringB);

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

    //testDFAs();

    //testAllAlphabetSizes("01", "2");

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



//gives probabilities for a few DFAs and loops through all possible alphabet sizes for each one
void testDFAs () {

    //format - stringA, stringB, alphabetSize
    //test0 - 00, 11, 2     (should be 1/2 for all sizes)
    //test1 - 12345, f, 6
    //test2 - 01, 1, 5      (should be 1/5 for size 5)
    //test3 - 01, 2, 5      (should be 1/6 for size 5)
    //test4 - abc, z, 6
    std::vector<std::string> stringA = {"00", "12345", "01", "01", "abc"};
    std::vector<std::string> stringB = {"11", "f", "1", "2", "z"};
    std::vector<int> alphabetSize = {2, 6, 5, 5, 6};

    //loop through each test case
    for (int i = 0; i < stringA.size(); i++) {
        std::cout << "\n\n********************************** Test " << i << " **********************************" << std::endl;

        //test out all possible alphabet sizes for that case
        for (int alphaSize = alphabetSize[i]; alphaSize <= 32; alphaSize++) {
            std::unordered_set<char> alphabet = setUpAlphabet(stringA[i], stringB[i], alphaSize);
            GameDFA test1 = GameDFA(stringA[i], stringB[i], alphabet);
            RecurrenceEquationBuilder builder1(&test1);
            std::cout << "\nFor A = " << stringA[i] << ", B = " << stringB[i] << ", with alphabet size " << alphaSize <<
                ", the probability of A winning is " << getProbabilityOfWin(test1.getStartState(), builder1) << std::endl;

        }

    }

}



//outputs probability of A winning for all possible alphabet sizes up to 32
void testAllAlphabetSizes(std::string stringA, std::string stringB) {

    //find out minimum alphabet size for this case
    std::unordered_set<char> uniqueLetters;
    for (auto i : stringA+stringB) {
        if (uniqueLetters.count(i) == 0) {
            uniqueLetters.insert(i);
        }
    }
    int currentAlphabetSize = uniqueLetters.size();

    //test out all possible alphabet sizes for this case
    for (currentAlphabetSize; currentAlphabetSize <= 32; currentAlphabetSize++) {
        std::unordered_set<char> alphabet = setUpAlphabet(stringA, stringB, currentAlphabetSize);
        GameDFA test1 = GameDFA(stringA, stringB, alphabet);
        RecurrenceEquationBuilder builder1(&test1);
        std::cout << "\nFor A = " << stringA << ", B = " << stringB << ", with alphabet size " << currentAlphabetSize <<
            ", the probability of A winning is " << getProbabilityOfWin(test1.getStartState(), builder1) << std::endl;

    }
}
