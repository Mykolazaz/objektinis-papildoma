#include "lib.h"
#include "fileAnalyzer.h"

int main() {
    std::string fileName;

    std::cout << "Here are all avaible files:" << "\n";
    system("ls *.txt");

    std::cout << "\nChoose a file to analyze: " << "\n";
    std::cin >> fileName;
    
    FileAnalyzer analyzer("tlds-alpha-by-domain.txt");

    try {
        int choice;
        std::cout << "Analyse words (0) or URLs (1)?"  << "\n";;
        std::cin >> choice;
        if (choice == 0) {
            analyzer.analyzeFile(fileName);
            analyzer.outputRepeatedWords("repeated_words.txt");
            analyzer.outputWordLocations("word_locations.txt");
            std::cout << "Results saved to repeated_words.txt and word_locations.txt" << "\n";;
        }
        else if (choice == 1) {
            analyzer.extractUrls(fileName);
            analyzer.printUniqueUrls();
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