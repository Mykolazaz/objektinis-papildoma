#ifndef FILE_ANALYZER_H
#define FILE_ANALYZER_H

class FileAnalyzer {
private:
    std::unordered_map<std::string, int> wordFrequency;
    std::unordered_map<std::string, std::vector<int>> wordLocations;
    std::set<std::string> uniqueUrls;
    std::set<std::string> validTlds;

    void loadValidTlds(const std::string& tldFilename);
    std::string cleanWord(const std::string& word);
    std::vector<std::string> splitIntoWords(const std::string& line);
    std::string extractBaseDomain(const std::string& url);
    bool isValidDomain(const std::string& domain);

public:
    FileAnalyzer(const std::string& tldFilename);
    
    void analyzeFile(const std::string& filename);
    void extractUrls(const std::string& filename);
    void outputRepeatedWords(const std::string& outputFilename);
    void outputWordLocations(const std::string& outputFilename);
    void printUniqueUrls(const bool& toFile = false);
};

#endif