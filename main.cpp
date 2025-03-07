#include "textfile.cpp"
#include "huffman.cpp"

#include <iostream>
#include <memory>
#include <map>
#include <set>

int main() {
    // Set containing text file extensions
    std::set<std::string> extensions = {
        ".txt", ".md", ".rtf", ".doc", ".docx", ".odt", ".tex", ".csv", 
        ".json", ".xml", ".yaml", ".yml", ".ini", ".log", ".html", ".css", 
        ".js", ".ts", ".sql"
    };

    std::cout << std::endl;
    std::cout << "|------------------------------------------------------|" << std::endl;
    std::cout << "|------------------------------------------------------|" << std::endl;
    std::cout << "|         Textfile Compressor CLI Application          |" << std::endl;
    std::cout << "|                Welcome to the app!                   |" << std::endl;
    std::cout << "|------------------------------------------------------|" << std::endl;
    std::cout << "|------------------------------------------------------|" << std::endl;
    std::cout << std::endl;

    while (true) {
        std::cout << "Please enter path to text file ('exit' to exit program): ";
        std::string filePath;
        std::getline(std::cin, filePath);

        if (filePath == "exit") {
            std::cout << "User has exited program" << std::endl;
            break;
        }

        // Separate extension file from path
        size_t separator = filePath.find_last_of(".");
        size_t pathLen = filePath.size();
        std::string filename = filePath.substr(0, separator);
        std::string ext = filePath.substr(separator, pathLen);

        if (extensions.find(ext) == extensions.end() && ext != "huff") {
            std::cerr << "Error: invalid file type" << std::endl;
            continue;
        }
        
        std::unique_ptr<TextFile> textFile = std::make_unique<TextFile>(filePath);

        textFile->loadContent();
        textFile->printCharMap();

        std::string content = textFile->getContent();
        std::map<char, int> charMap = textFile->getCharMap();
        std::vector<char> arr;
        std::vector<int> freq;

        for (const auto& pair : charMap) {
            arr.push_back(pair.first);
            freq.push_back(pair.second);
        }
        
        std::unique_ptr<HuffmanTree> huffmanTree = std::make_unique<HuffmanTree>(
            arr, 
            freq,
            filename
        );
        
        huffmanTree->createTree();
        huffmanTree->printCodeMap();
        huffmanTree->printDecodeMap();
        huffmanTree->encodeFile(content);
        huffmanTree->decodeFile();
        huffmanTree->printDecompress();
    }

    return 0;
}