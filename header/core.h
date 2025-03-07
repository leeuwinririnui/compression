#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <fstream>
#include <bitset>
#include <iostream>
#include <fstream>
#include <map>

struct MinHeapNode {
    char data;
    int freq;
    MinHeapNode *left, *right;

    MinHeapNode(char data, int freq) 
        : data(data), freq(freq), left(nullptr), right(nullptr) {};
    
    void displayData() { std::cout << freq << " : " << data << std::endl; }
};

// Functor for comparing freq of two nodes 
struct Compare {
    bool operator()(MinHeapNode* left, MinHeapNode* right) {
        return (left->freq > right->freq);
    }
};

class HuffmanTree {
private:
    std::string filename;
    std::string extension = ".huff";
    std::vector<char> data; 
    std::vector<int> freq;
    std::unordered_map<char, std::string> codeMap;
    std::unordered_map<std::string, char> decodeMap;
    std::string encodedData;
    std::string decodedData;
    int leadingZeros;
public:
    HuffmanTree(std::vector<char> data, std::vector<int> freq, std::string filename) 
        : data(data), freq(freq), filename(filename) {}

    ~HuffmanTree() { std::cout << "Destructor called for Huffman Tree" << std::endl; }

    void saveState();
    void createCodes(MinHeapNode* root, std::string str);
    void createTree();
    void encodeFile(std::string content);
    void decodeFile();
    void printCodeMap();
    void printDecodeMap();
    void printDecompress();

    std::unordered_map<char, std::string> getCodeMap() { return codeMap; }
    std::unordered_map<std::string, char> getDecodeMap() { return decodeMap; }
    std::string getEncodedData() { return encodedData; }
    std::string getDecodedData() { return decodedData; }
};

class TextFile {
private:
    std::string path;
    std::string content;
    std::map<char, int> charMap;
public:
    TextFile(std::string path) : path(path) {}

    ~TextFile() { std::cout << "Destructor called for file " << path << std::endl; }

    void loadContent();
    void printCharMap();

    const std::string& getContent() { return content; }
    int getLength() { return content.size(); }
    const std::map<char, int>& getCharMap() { return charMap; };
};

#endif