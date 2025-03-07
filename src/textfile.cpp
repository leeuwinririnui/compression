#include "core.h"

void TextFile::loadContent() {
    std::ifstream file(path);
    char c;

    if (!file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        return;
    }
    
    // Store each char as key in map with frequency as value
    while (file.get(c)) {
        content += c;
        charMap[c]++;
    }

    file.close();
}

void TextFile::printCharMap() {
    for (const auto& c : charMap) {
        std::cout << c.first << ": " << c.second << std::endl;
    }
    std::cout << std::endl;
}