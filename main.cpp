#include "lib.h"
#include "fileAnalyzer.h"

int main() {
    std::string fileName;

    std::cout << "Here are all avaible files:" << std::endl;
    system("ls *.txt");
    std::cout << "\nChoose a file to analyze: " << std::endl;
    std::cin >> fileName;
    

    FileAnalyzer analyzer("tlds-alpha-by-domain.txt");

    try {
        analyzer.analyzeFile("vilnius-wiki.txt");
        analyzer.outputRepeatedWords("repeated_words.txt");
        analyzer.outputWordLocations("word_locations.txt");
        analyzer.extractUrls("vilnius-wiki.txt");
        analyzer.printUniqueUrls();
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}