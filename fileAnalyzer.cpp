#include "lib.h"
#include "fileAnalyzer.h"

std::string FileAnalyzer::cleanWord(const std::string& word) {
    std::string cleaned;
    for (char c : word) {
        if (std::isalpha(c)) {
            cleaned += std::tolower(c);
        }
    }
    return cleaned;
}

std::vector<std::string> FileAnalyzer::splitIntoWords(const std::string& line) {
    std::vector<std::string> words;
    std::string currentWord;
    
    for (char c : line) {
        if (std::isalpha(c)) {
            currentWord += c;
        } else if (!currentWord.empty()) {
            words.push_back(currentWord);
            currentWord.clear();
        }
    }
    
    if (!currentWord.empty()) {
        words.push_back(currentWord);
    }
    
    return words;
}

void FileAnalyzer::loadValidTlds(const std::string& tldFilename) {
    std::ifstream file(tldFilename);
    if (!file.is_open()) {
        std::cerr << "Error opening TLD file: " << tldFilename << "\n";
        return; // File not found or cannot be opened
    }

    std::string tld;
    while (std::getline(file, tld)) {
        if (!tld.empty() && tld[0] != '#') { // Skip comments
            // Convert all TLDs to lowercase for case-insensitive comparison
            std::transform(tld.begin(), tld.end(), tld.begin(), ::tolower);
            validTlds.insert(tld);
        }
    }
    file.close();
}

std::string FileAnalyzer::extractBaseDomain(const std::string& url) {
    std::string baseDomain = url;
    
    // Remove http:// or https://
    auto protocolPos = baseDomain.find("://");
    if (protocolPos != std::string::npos) {
        baseDomain = baseDomain.substr(protocolPos + 3);
    }
    
    // Remove the part after the first slash or special characters
    auto firstSlash = baseDomain.find_first_of("/?#");
    if (firstSlash != std::string::npos) {
        baseDomain = baseDomain.substr(0, firstSlash);
    }
    
    return baseDomain;
}

bool FileAnalyzer::isValidDomain(const std::string& domain) {
    if (domain.empty() || // Empty domain
        domain[0] == '.' || // Starts with a dot
        domain[0] == '-' || // Starts with a hyphen
        domain.back() == '.' || // Ends with a dot
        domain.back() == '-' // Ends with a hyphen
        ) {
        return false;
    }

    // Split domain into parts: www.example.com -> [www, example, com]
    std::vector<std::string> parts;
    std::string part;
    std::istringstream domainStream(domain);
    
    while (std::getline(domainStream, part, '.')) {
        if (part.empty()) return false;
        parts.push_back(part);
    }

    if (parts.size() < 2) return false;  // Need at least two parts

    // Get the TLD (last part)
    std::string tld = parts.back();
    std::transform(tld.begin(), tld.end(), tld.begin(), ::tolower);

    // Check if TLD exists in valid TLDs set
    return validTlds.find(tld) != validTlds.end();
}


FileAnalyzer::FileAnalyzer(const std::string& tldFilename) {
    loadValidTlds(tldFilename);
    if (validTlds.empty()) {
        std::cerr << "Warning: No valid TLDs loaded." << "\n";
    }
}

void FileAnalyzer::analyzeFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << "\n";
        return;
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        lineNumber++;
        std::vector<std::string> words = splitIntoWords(line);
        for (const std::string& word : words) {
            std::string cleanedWord = cleanWord(word);
            
            if (!cleanedWord.empty() && cleanedWord.length() > 1) {
                wordFrequency[cleanedWord]++;
                allWords.push_back(cleanedWord);
                
                if (std::find(wordLocations[cleanedWord].begin(), 
                            wordLocations[cleanedWord].end(), 
                            lineNumber) == wordLocations[cleanedWord].end()) {
                    wordLocations[cleanedWord].push_back(lineNumber);
                }
            }
        }
    }
    
    file.close();
}

void FileAnalyzer::findWordsContaining(const std::string& substring) {
    std::string searchStr = cleanWord(substring);
    if (searchStr.empty()) {
        std::cout << "Search string is empty after cleaning." << std::endl;
        return;
    }

    std::cout << "Words containing '" << searchStr << "':" << std::endl;
    bool found = false;
    
    int matchCounter = 0;

    for (const std::string& word : allWords) {
        if (word.find(searchStr) != std::string::npos) {
            matchCounter++;
            std::cout << word << std::endl;
            found = true;
        }
    }

    std::cout << "Found number of matches: " << matchCounter << "\n";

    if (!found) {
        std::cout << "No words found containing '" << searchStr << "'" << std::endl;
    }
}

void FileAnalyzer::outputRepeatedWords(const std::string& outputFilename) {
    std::ofstream outFile(outputFilename);
    if (!outFile.is_open()) {
        std::cerr << "Error creating output file: " << outputFilename << "\n";
        return;
    }

    std::vector<std::pair<std::string, int>> sortedWords;
    for (const auto& entry : wordFrequency) {
        if (entry.second > 1) {
            sortedWords.push_back(entry);
        }
    }

    std::sort(sortedWords.begin(), sortedWords.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second || 
                    (a.second == b.second && a.first < b.first);
        });

    for (const auto& entry : sortedWords) {
        outFile << entry.first << ": " << entry.second << "\n";
    }
    
    outFile.close();
}

void FileAnalyzer::outputWordLocations(const std::string& outputFilename) {
    std::ofstream outFile(outputFilename);
    if (!outFile.is_open()) {
        std::cerr << "Error creating output file: " << outputFilename << "\n";
        return;
    }

    std::vector<std::pair<std::string, std::vector<int>>> sortedLocations;
    for (const auto& entry : wordLocations) {
        if (wordFrequency[entry.first] > 1) {
            sortedLocations.push_back(entry);
        }
    }

    std::sort(sortedLocations.begin(), sortedLocations.end(),
        [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

    for (const auto& entry : sortedLocations) {
        outFile << entry.first << " appears on lines: ";
        std::vector<int> lines = entry.second;
        std::sort(lines.begin(), lines.end());
        for (int line : lines) {
            outFile << line << " ";
        }
        outFile << "\n";
    }
    
    outFile.close();
}

void FileAnalyzer::extractUrls(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << "\n";
        return;
    }

    std::regex url_pattern( // Regex of URL patterns
        R"((https?:\/\/)?([a-zA-Z0-9]([a-zA-Z0-9-]*[a-zA-Z0-9])?\.)+[a-zA-Z]{2,}(/[^\s]*)?)",
        std::regex::icase
    );

    std::string line;
    while (std::getline(file, line)) {
        auto words_begin = std::sregex_iterator(line.begin(), line.end(), url_pattern);
        auto words_end = std::sregex_iterator();

        // Iterate over all regex matches in the same line
        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            std::smatch match = *i;
            std::string potentialUrl = match.str();
            std::string baseDomain = extractBaseDomain(potentialUrl);
            
            if (isValidDomain(baseDomain)) {
                uniqueUrls.insert(baseDomain);
            }
        }
    }
    
    file.close();
}

void FileAnalyzer::printUniqueUrls(const bool& toFile) {
    if (uniqueUrls.empty()) {
        std::cout << "No valid URLs found in the file."  << "\n";
        return;
    }

    if (toFile) {
        std::ofstream outFile("unique_urls.txt");
        if (!outFile.is_open()) {
            std::cerr << "Error creating output file: unique_urls.txt" << "\n";
            return;
        }

        for (const auto& url : uniqueUrls) {
            outFile << url << "\n";
        }
        outFile.close();

        std::cout << "Results saved to unique_urls.txt"  << "\n";
    } else {
        std::cout << "Unique base domains found:"  << "\n";
        for (const auto& url : uniqueUrls) {
            std::cout << url  << "\n";
        }
    }
}