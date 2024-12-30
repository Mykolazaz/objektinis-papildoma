#include "lib.h"
#include "fileAnalyzer.h"

int main() {
    std::string fileName;

    std::cout << "Here are all avaible files:" << "\n";
    system("ls *.txt");

    std::cout << "\n" << "Choose a file to analyze: " << "\n";
    std::cin >> fileName;
    
    FileAnalyzer analyzer("tlds-alpha-by-domain.txt");

    try {
        int choice1, choice2, choice3;
        std::cout << "Analyse words (0) or URLs (1)?"  << "\n";;
        std::cin >> choice1;

        if (choice1 == 0) {
            std::cout << "Analyse words containing a substring (0) or find repeated words (1)?" << "\n";
            std::cin >> choice3;

            if (choice3 == 0) {
                std::string substring;
                std::cout << "Enter a substring to search for: " << "\n";
                std::cin >> substring;

                analyzer.analyzeFile(fileName);
                analyzer.findWordsContaining(substring);
            }
            else if (choice3 == 1) {

            analyzer.analyzeFile(fileName);
            analyzer.outputRepeatedWords("repeated_words.txt");
            analyzer.outputWordLocations("word_locations.txt");
            std::cout << "Results saved to repeated_words.txt and word_locations.txt" << "\n";;
            }
        }
        else if (choice1 == 1) {
            std::cout << "Output to console (0) or to file (1)?"  << "\n";
            std::cin >> choice2;

            analyzer.extractUrls(fileName);
            analyzer.printUniqueUrls(choice2);
        }
        else {
            std::cout << "Invalid choice"  << "\n";
            EXIT_FAILURE;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what()  << "\n";
        return 1;
    }

    return 0;
}