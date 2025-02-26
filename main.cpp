#include <iostream>
#include <fstream>
#include <memory>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <queue>

// Nodes for building min heap
struct MinHeapNode {
    char data;
    int freq;
    MinHeapNode *left, *right;

    MinHeapNode(char data, int freq) 
        : data(data), freq(freq), left(nullptr), right(nullptr) {};
    
    void displayData() { std::cout << freq << " : " << data << std::endl; }
};

// Functor (Function object) for comparing freq of two nodes 
struct Compare {
    // Overload the operator() so that it behaves as a function when called
    bool operator()(MinHeapNode* left, MinHeapNode* right) {
        return (left->freq > right->freq);
    }
};

// Class for building binary tree and encoding string
class HuffmanTree {
private:
    std::vector<char> data; 
    std::vector<int> freq;
    std::unordered_map<char, std::string> codeMap;
    std::string encodedData;
public:
    HuffmanTree(std::vector<char> data, std::vector<int> freq) 
        : data(data), freq(freq) {}

    ~HuffmanTree() { std::cout << "Destructor called for Huffman Tree\n"; }

    void printCodes() {
        for (const auto& code : codeMap) {
            std::cout << code.first << ": " << code.second << "\n"; 
        }

        std::cout << "\n";
    }

    void createCodes(MinHeapNode* root, std::string str) {
        if (!root) return;

        if (root->data != '\0') codeMap[root->data] = str;

        createCodes(root->left, str + "0");
        createCodes(root->right, str + "1");
    }

    // Main function to build Huffman Tree
    void createTree() {
        MinHeapNode *left, *right, *top;
    
        // Create min heap and insert all characters of data
        // Stores pointers to MinHeapNode using std::vector as internal storage (default)
        // Sort elements using Compare functor
        std::priority_queue<MinHeapNode*, std::vector<MinHeapNode*>, Compare> minHeap;
        for (int i = 0; i < data.size(); ++i) {
            minHeap.push(new MinHeapNode(data[i], freq[i]));
        } 

        // Last remaining element in heap will be root
        while (minHeap.size() != 1) {
            // Extract two minimum freq nodes from heap
            left = minHeap.top();
            minHeap.pop();

            right = minHeap.top();
            minHeap.pop();

            // Create a new internal node by adding the two node frequencies
            // Make left and right children of this new node. Use '\0' (null character)
            top = new MinHeapNode('\0', left->freq + right->freq);
            top->left = left;
            top->right = right;
            
            minHeap.push(top);
        }

        // Create Huffman codes using the binary tree built above
        createCodes(minHeap.top(), "");
    }

    // Function to encode string and right to file
    void encodeFile(std::string content, std::string name) {
        for (const auto& c : content) {
            if (codeMap.find(c) != codeMap.end()) {
                encodedData += codeMap[c];
            }
        }

        // Write encoded string to file
        std::ofstream outfile("encoded_" + name);
        outfile << encodedData;
    }
};

class TextFile {
private:
    std::string path;
    std::string content;
    std::map<char, int> charMap;
public:
    TextFile(std::string path) : path(path) {}

    ~TextFile() { std::cout << "Destructor called for file " << path << "\n"; }

    const std::string& getContent() { return content; }
    int getLength() { return content.size(); }
    const std::map<char, int>& getCharMap() { return charMap; };

    void loadContent() {
        std::ifstream file(path);
        char c;
    
        if (!file.is_open()) {
            std::cerr << "Unable to open file\n";
            return;
        }
        
        // Store each char as key in map with frequency as value
        while (file.get(c)) {
            content += c;
            charMap[c]++;
        }
    
        file.close();
    }
    
    void printCharMap() {
        for (const auto& c : charMap) {
            std::cout << c.first << ": " << c.second << "\n";
        }
        std::cout << "\n";
    }
    
};

int main() {
    std::unique_ptr<TextFile> textFile = std::make_unique<TextFile>("read.txt");

    textFile->loadContent();
    textFile->printCharMap();

    std::string content = textFile->getContent();
    // std::cout << textFile->getContent() << "\n" << textFile->getLength() << "\n\n";

    std::map<char, int> charMap = textFile->getCharMap();
    std::vector<char> arr;
    std::vector<int> freq;

    for (const auto& pair : charMap) {
        arr.push_back(pair.first);
        freq.push_back(pair.second);
    }
    
    std::unique_ptr<HuffmanTree> huffmanTree = std::make_unique<HuffmanTree>(
        arr, 
        freq
    );
    
    huffmanTree->createTree();
    huffmanTree->printCodes();
    huffmanTree->encodeFile(content, "read.txt");

    return 0;
}