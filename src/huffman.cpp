#include "core.h"

// Function to write file state information in JSON format
void HuffmanTree::saveState() {
    
}

// Helper function to create Huffman codes
void HuffmanTree::createCodes(MinHeapNode* root, std::string str) {
    if (!root) 
        return;
    
    if (root->data != '\0') { 
        codeMap[root->data] = str;
        decodeMap[str] = root->data;
    }

    createCodes(root->left, str + "0");
    createCodes(root->right, str + "1");
}

// Main function to build Huffman Tree
void HuffmanTree::createTree() {
    if (data.size() == 0 || freq.size() == 0)
        return;

    // Check if there is only one unique character in the data
    if (data.size() == 1) {
        // Assign a huffman code and return
        codeMap[data[0]] = "0";
        decodeMap["0"] = data[0];
        return;
    }

    MinHeapNode *left, *right, *top; 

    // Create min heap and insert all characters of data
    // Stores pointers to MinHeapNode using std::vector of as internal storage (default)
    // Sort elements using Compare functor
    std::priority_queue<MinHeapNode*, std::vector<MinHeapNode*>, Compare> minHeap;
    for (size_t i = 0; i < data.size(); i++) {
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

// Function to encode string and write to file
void HuffmanTree::encodeFile(std::string content) {
    // Encode data to std::string
    for (const auto& c : content) {
        if (codeMap.find(c) != codeMap.end()) {
            encodedData += codeMap[c];
        } else {
            std::cerr << "Error! invalid data " << c << std::endl;
            encodedData = "";
        }
    }

    std::fstream outfile;

    outfile.open(filename + extension, std::ios::out | std::ios::binary);

    if (outfile) {
        // Process binary string 8 bits at a tie (1 byte)
        for (size_t i = 0; i < encodedData.length(); i += 8) {
            // Retrieve 8 bit substring
            std::string strByte = encodedData.substr(i, 8);
            
            // Keep track of total leading zeros used
            if (strByte.length() < 8) 
                leadingZeros = 8 - strByte.length();

            std::cout << strByte.length() << " ";

            // Read string as binary number (constructor expects only 0 and 1 characters)
            std::bitset<8> encodedBitset(strByte);

            // Convert bitset to an unsigned long (bitset doesnt have a conversion method to 8 bit)
            unsigned long longBitset = encodedBitset.to_ulong();
                
            // Treat address of longBitset as a pointer to a char
            unsigned char byte = static_cast<char>(longBitset);
            
            // Write byte into file - sizeof(byte) = 1 (byte)
            outfile.write(reinterpret_cast<char*>(&byte), sizeof(byte));

            std::cout << "Written bits: " << std::bitset<8>(byte) << std::endl;
        }

        outfile.close();

        std::cout << "File written successfully" << std::endl;
    } else {
        std::cout << "Error opening file!" << std::endl;
    }
}

// Function to decode data from file 
void HuffmanTree::decodeFile() {
    // Open file in binary mode and store data in internal buffer
    std::fstream infile(filename + extension, std::ios::in | std::ios::binary);
    
    if (!infile) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    // Unsigned char to hold each byte of data retrieved from internal buffer
    unsigned char byte;

    // Size of data will always be 1 bit
    std::vector<unsigned char> bytes; 

    // Read compressed binary file
    while (infile.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        // Convert byte to long
        bytes.push_back(byte);
    }

    for (size_t i = 0; i < bytes.size(); i++) {
        // Convert long values representing each byte to bitset
        std::bitset<8> decodedByte(bytes[i]);
        
        std::string dataToAdd = decodedByte.to_string();

        // Check if last byte has leading zeros
        if (i == bytes.size() - 1 && leadingZeros > 0) {
            dataToAdd = dataToAdd.substr(leadingZeros, 8);
        }

        // Convert bitset to string
        decodedData += dataToAdd;
    }
    
    for (size_t i = 0; i < decodedData.size(); i+=8) {
        std::cout << decodedData.substr(i, 8) << " ";
    }

    infile.close();

    std::cout << std::endl;
}

void HuffmanTree::printCodeMap() {
    for (const auto& code : codeMap) {
        std::cout << code.first << ": " << code.second << std::endl; 
    }

    std::cout << std::endl;
}

void HuffmanTree::printDecodeMap() {
    for (const auto& code : decodeMap) {
        std::cout << code.first << ": " << code.second << std::endl; 
    }

    std::cout << std::endl;
}

void HuffmanTree::printDecompress() {
    std::string code;
    std::string decodedText;

    for (size_t i = 0; i < decodedData.size(); i++) {
        code += decodedData[i];
        if (decodeMap.find(code) != decodeMap.end()) {
            decodedText += decodeMap[code];
            code = "";
        }
    }

    std::cout << std::endl << decodedText << std::endl << std::endl;
}