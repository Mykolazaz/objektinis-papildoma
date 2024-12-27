#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <regex>
#include <set>

class FileAnalyzer {
private:
    std::unordered_map<std::string, int> wordFrequency;
    std::unordered_map<std::string, std::vector<int>> wordLocations;
    std::set<std::string> uniqueUrls; // Store unique URLs
    std::set<std::string> validTlds; // Store unique valid TLDs

    void loadValidTlds(const std::string& tldFilename) {
        std::ifstream file(tldFilename);
        if (!file.is_open()) {
            std::cerr << "Error opening TLD file: " << tldFilename << std::endl;
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

    std::string extractBaseDomain(const std::string& url) {
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

    bool isValidDomain(const std::string& domain) {
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

public:
    FileAnalyzer(const std::string& tldFilename) {
        loadValidTlds(tldFilename);
        if (validTlds.empty()) {
            std::cerr << "Warning: No valid TLDs loaded." << std::endl;
        }
    }

    void extractUrls(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        std::regex url_pattern(
            R"((https?:\/\/)?([a-zA-Z0-9]([a-zA-Z0-9-]*[a-zA-Z0-9])?\.)+[a-zA-Z]{2,}(/[^\s]*)?)",
            std::regex::icase
        );

        std::string line;
        while (std::getline(file, line)) {
            auto words_begin = std::sregex_iterator(line.begin(), line.end(), url_pattern);
            auto words_end = std::sregex_iterator();

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
};

int main(){
    return 0;
}