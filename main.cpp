#include <iostream>
#include <fstream>
#include <memory>
#include <map>
#include <vector>

struct Node {
    int count;
    char letter;

    Node(int count, char letter) : count(count), letter(letter) {}
    void displayData() { std::cout << count << " : " << letter << std::endl; }
};

class TextFile {
private:
    std::string path;
    std::string content;
    int length;
    std::map<char, int> data;
    std::vector<Node> nodes;
public:
    TextFile(std::string path) : path(path) {}
    ~TextFile() { std::cout << "Destructor Called for file " << path << std::endl; }

    void loadContent();
    void createTree();

    std::string getContent() { return content; }
    int getLength() { return length; }
    std::map<char, int> getData() { return data; }
};

int main() {
    std::unique_ptr<TextFile> textFile = std::make_unique<TextFile>("read.txt");

    textFile->loadContent();
    std::cout << textFile->getContent() << "\n" << textFile->getLength() << "\n";
    for (const auto& pair : textFile->getData()) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << "\n";
    }
    textFile->createTree();

    return 0;
}

void TextFile::loadContent() {
    std::ifstream file(path);
    char ch;

    if (!file.is_open()) {
        std::cerr << "Unable to open file\n";
        return;
    }
    

    while (file.get(ch)) {
        content += ch;
        data[ch]++;
    }
    length = content.size();

    file.close();
}

void TextFile::createTree() {
    if (data.empty()) {
        std::cerr << "No data found!\n";
        return; 
    }

    for (const auto& d : data) {
        nodes.push_back(Node(d.second, d.first));
    }

    for (auto& n : nodes) {
        n.displayData();
    }
}