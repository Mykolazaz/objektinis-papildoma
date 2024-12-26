#include <iostream>
#include <unordered_map>
#include <set>

class FileAnalyzer {
private:
    std::unordered_map<std::string, int> wordFrequency;
    std::unordered_map<std::string, std::vector<int>> wordLocations;
    std::set<std::string> uniqueUrls;
    std::set<std::string> validTlds;
};

int main(){
    return 0;
}