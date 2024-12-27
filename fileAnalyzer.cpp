#include <iostream>
#include <fstream>
#include <unordered_map>
#include <set>

class FileAnalyzer {
private:
    std::unordered_map<std::string, int> wordFrequency;
    std::unordered_map<std::string, std::vector<int>> wordLocations;
    std::set<std::string> uniqueUrls;
    std::set<std::string> validTlds;

    void loadValidTlds(const std::string& tldFilename) {
        std::ifstream file(tldFilename);
        if (!file.is_open()) {
            std::cerr << "Error opening TLD file: " << tldFilename << std::endl;
            return;
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
        size_t protocolPos = baseDomain.find("://");
        if (protocolPos != std::string::npos) {
            baseDomain = baseDomain.substr(protocolPos + 3);
        }
        
        // Remove the part after the first slash or special characters
        size_t firstSlash = baseDomain.find_first_of("/?#");
        if (firstSlash != std::string::npos) {
            baseDomain = baseDomain.substr(0, firstSlash);
        }
        
        return baseDomain;
    }
};

int main(){
    return 0;
}