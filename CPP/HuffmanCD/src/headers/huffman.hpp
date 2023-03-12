#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

// ============================================================================================================
// = STRUCTS ==================================================================================================
// ============================================================================================================

struct InfoByte
{
    uint8_t symbol;
    uint64_t n;             // Amount of times each byte is repeated
    uint64_t vsize;         // Size of the vector
    std::vector<bool> code; // Code
};

struct HuffmanTreeNode
{
    uint8_t symbol;
    uint64_t freq;
    HuffmanTreeNode *left;
    HuffmanTreeNode *right;

    HuffmanTreeNode() : symbol('\0'), freq(0), left(nullptr), right(nullptr){};
    HuffmanTreeNode(uint8_t symbol, uint64_t freq, HuffmanTreeNode *left = nullptr, HuffmanTreeNode *right = nullptr)
        : symbol(symbol), freq(freq), left(left), right(right){};

    bool operator<(const HuffmanTreeNode &other) const
    {
        return freq < other.freq;
    }
};

struct HuffmanTreeInfo
{
    uint64_t numNodes;
    uint64_t depth;

    HuffmanTreeInfo(uint64_t numNodes, uint64_t depth)
        : numNodes(numNodes), depth(depth){};
};

class BitWriter
{
private:
    FILE *&outputFile;
    uint8_t buffer;
    int count;
    uint32_t bitLength;

public:
    BitWriter(FILE *&outputFile) : outputFile(outputFile), buffer(0), count(0), bitLength(0) {}

    void writeBit(bool bit)
    {
        buffer <<= 1;
        buffer |= bit;
        count++;
        if (count == 8)
            saveBuffer();
    }

    void saveBuffer()
    {
        fwrite(&buffer, sizeof(buffer), 1, outputFile);
        buffer = 0;
        count = 0;
    }

    void flush()
    {
        if (count > 0)
        {
            buffer <<= (8 - count);
            saveBuffer();
        }
    }

    void writeBitLength()
    {
        fwrite(&bitLength, sizeof(uint32_t), 1, outputFile);
    }

    void setBitLength(uint32_t length)
    {
        bitLength = length;
    }
};

// ============================================================================================================
// = COMPRESSION ==============================================================================================
// ============================================================================================================

bool fByteCounter(std::string &path, std::vector<InfoByte> &arr)
{
    FILE *f = fopen(path.c_str(), "r+b");

    if (!f)
    {
        std::cerr << "Failed to open " << path << std::endl;
        return 1;
    }

    // Count
    uint8_t symbol;
    while (fread(&symbol, sizeof(symbol), 1, f))
    {
        if (symbol >= arr.size())
        {
            int i = arr.size();
            while (i <= symbol)
            {
                arr.push_back({0, 0, 0, {}});
                i++;
            }
        }
        arr[symbol].n++;
        arr[symbol].symbol = symbol;
    }

    // Close file
    fclose(f);
    return 0;
}

std::list<HuffmanTreeNode> *fOrderedList(std::vector<InfoByte> &arr)
{
    std::list<HuffmanTreeNode> *list = new std::list<HuffmanTreeNode>();
    for (int i = 0; i < arr.size(); i++)
    {
        if (arr[i].n > 0)
        {
            uint8_t symbol = i;
            HuffmanTreeNode node = HuffmanTreeNode(symbol, arr[i].n);
            auto it = std::upper_bound(list->begin(), list->end(), node);
            list->insert(it, node);
        }
    }

    return list;
}

HuffmanTreeNode *fListToTree(std::list<HuffmanTreeNode> *list)
{
    // Loop until there is only one node left in the list
    while (list->size() > 1)
    {
        // Pop the first two nodes from the list
        HuffmanTreeNode *node1 = new HuffmanTreeNode(list->front());
        list->pop_front();
        HuffmanTreeNode *node2 = new HuffmanTreeNode(list->front());
        list->pop_front();

        // Create a new node with the sum of the frequencies of node1 and node2
        HuffmanTreeNode newNode = HuffmanTreeNode('\0', node1->freq + node2->freq, node1, node2);

        // Insert the new node back into the list, maintaining the order
        auto it = std::upper_bound(list->begin(), list->end(), newNode);
        list->insert(it, newNode);
    }

    // Return the last node in the list as the root of the tree
    HuffmanTreeNode *root = new HuffmanTreeNode(list->front());
    return root;
}

bool _fGenerateHuffmanCode(HuffmanTreeNode *node, std::vector<bool> &code, std::vector<InfoByte> &arr)
{
    if (!node)
        return false;

    // Leaf
    if (node->left == nullptr && node->right == nullptr)
    {
        if (node->symbol >= arr.size())
            return false;

        arr[node->symbol].code = code;
        arr[node->symbol].vsize = code.size();
        return true;
    }

    code.push_back(false); // 0 is left
    _fGenerateHuffmanCode(node->left, code, arr);
    code.pop_back(); // Removes the last false as it is nullptr

    code.push_back(true);
    _fGenerateHuffmanCode(node->right, code, arr);
    code.pop_back(); // Removes the last true as it is nullptr

    return true;
}

bool fGenerateHuffmanCode(HuffmanTreeNode *&root, std::vector<InfoByte> &arr)
{
    std::vector<bool> code;

    if (!root)
        return false;

    _fGenerateHuffmanCode(root, code, arr);

    return true;
}

void fWriteHeader(FILE *&outputFile, std::vector<InfoByte> &arr)
{
    // Write the array size
    int arraysize = arr.size();
    fwrite(&arraysize, sizeof(int), 1, outputFile);

    // Write the frequency table to the output file in binary format
    for (int i = 0; i < arr.size(); i++)
    {
        fwrite(&arr[i].symbol, sizeof(uint8_t), 1, outputFile);
        fwrite(&arr[i].n, sizeof(uint64_t), 1, outputFile);
        fwrite(&arr[i].vsize, sizeof(uint64_t), 1, outputFile);

        // Read the code variable one bool at a time
        if (arr[i].vsize == 0)
        {
            std::vector<bool> v = {};
            fwrite(&v, sizeof(v), 1, outputFile);
            continue;
        }
        for (int j = 0; j < arr[i].vsize; j++)
        {
            bool b = arr[i].code[j];
            fwrite(&b, sizeof(bool), 1, outputFile);
        }
    }
}

void fSaveCompressedFile(std::string &path, std::vector<InfoByte> &arr)
{
    std::string outputPath = path + ".huf";

    FILE *inputFile = fopen(path.c_str(), "r+b");

    if (inputFile == nullptr)
    {
        std::cerr << "Failed to open input file." << std::endl;
        return;
    }

    FILE *outputFile = fopen(outputPath.c_str(), "w+b");
    if (outputFile == nullptr)
    {
        std::cerr << "Failed to output input file." << std::endl;
        return;
    }

    // Write header
    fWriteHeader(outputFile, arr);

    // Create a BitWriter object to write the encoded data to the output file
    BitWriter bitWriter(outputFile);

    // Go to the start of the input file
    fseek(inputFile, 0, SEEK_SET);

    // Encode each symbol in the input file using Huffman coding
    uint8_t symbol;
    while (fread(&symbol, sizeof(uint8_t), 1, inputFile) == 1)
    {
        // Get the Huffman code for the symbol
        std::vector<bool> code = arr[symbol].code;

        // Write the length of the code to the BitWriter object
        uint32_t sizeOfPath = code.size();
        bitWriter.setBitLength(sizeOfPath);
        bitWriter.writeBitLength();

        // Write the code to the output file using the BitWriter object
        for (bool bit : code)
        {
            bitWriter.writeBit(bit);
        }

        bitWriter.flush();
    }

    // Flush any remaining bits in the BitWriter buffer to the output file
    bitWriter.flush();

    // Close the input and output files
    fclose(inputFile);
    fclose(outputFile);
}

void fCompress(std::string &path, void (*fDisplayProgress)(int))
{
    // Declaring the array for the bytes
    std::vector<InfoByte> arr;

    // Step 1: Count how many times each byte appears
    fByteCounter(path, arr);

    fDisplayProgress(20); // Step 1 of 5

    // Step 2: Create a list and order it
    std::list<HuffmanTreeNode> *list = fOrderedList(arr);

    fDisplayProgress(40); // Step 2 of 5

    // Step 3: Convert the list into a tree
    HuffmanTreeNode *root = fListToTree(list);
    free(list);

    fDisplayProgress(60); // Step 3 of 5

    // Step 4: Go through the tree and save each code inside an array
    fGenerateHuffmanCode(root, arr);
    free(root);

    fDisplayProgress(80); // Step 4 of 5

    // Step 5: Go through the original files and save each bit in the compressed file
    fSaveCompressedFile(path, arr);

    fDisplayProgress(100); // Step 5 of 5
}

// ============================================================================================================
// = DECOMPRESSION ============================================================================================
// ============================================================================================================

void fRebuildTree(FILE *&f, HuffmanTreeNode *&root)
{
    int arraysize;
    fread(&arraysize, sizeof(int), 1, f);

    // Read the frequency table from the header
    std::vector<InfoByte> arr;
    for (int i = 0; i < arraysize; i++)
    {
        arr.push_back({0, 0, 0, {}});
    }

    for (int i = 0; i < arraysize; i++)
    {
        fread(&arr[i].symbol, sizeof(uint8_t), 1, f);
        fread(&arr[i].n, sizeof(uint64_t), 1, f);
        fread(&arr[i].vsize, sizeof(uint64_t), 1, f);

        // Read the code variable one bool at a time
        if (arr[i].vsize == 0)
        {
            std::vector<bool> v = {};
            fread(&v, sizeof(v), 1, f);
            continue;
        }
        for (int j = 0; j < arr[i].vsize; j++)
        {
            bool b;
            fread(&b, sizeof(bool), 1, f);
            arr[i].code.push_back(b);
        }
    }

    for (int i = 0; i < arr.size(); i++)
    {
        // If it has a frequency
        if (arr[i].n > 0)
        {
            HuffmanTreeNode *node = root;

            for (size_t j = 0; j < arr[i].code.size(); j++)
            {

                if (arr[i].code[j])
                {
                    if (node->right == nullptr)
                        node->right = new HuffmanTreeNode('\0', 0);
                    node = node->right;
                }
                else
                {
                    if (node->left == nullptr)
                        node->left = new HuffmanTreeNode('\0', 0);
                    node = node->left;
                }
            }

            node->symbol = arr[i].symbol;
            node->freq = arr[i].n;
        }
    }
}

void fReadDecodeCreate(FILE *&f, HuffmanTreeNode *&root, std::string &path)
{
    // Open the output file for writing
    std::string outputPath = path.substr(0, path.length() - 4);
    size_t poslastdot = outputPath.rfind('.');
    outputPath = path.substr(0, poslastdot) + "_decompressed" + outputPath.substr(poslastdot);

    FILE *outputFile = fopen(outputPath.c_str(), "w+b");
    if (outputFile == nullptr)
    {
        std::cerr << "Failed to output input file." << std::endl;
        return;
    }

    // Traverse the Huffman tree to decode the input data
    HuffmanTreeNode *node = root;
    uint32_t sizeOfPath;
    while (fread(&sizeOfPath, sizeof(uint32_t), 1, f))
    {
        bool broken;
        for (int i = 0; i < sizeOfPath; i++)
        {
            broken = false;

            uint8_t byte;
            fread(&byte, sizeof(uint8_t), 1, f);

            for (int j = 7; j >= 0 && !broken; j--)
            {
                bool bit = ((byte >> j) & 1);

                if (bit)
                    node = node->right;
                else
                    node = node->left;

                i++;

                if (node == nullptr)
                    std::cout << std::endl;

                if (node->left == nullptr && node->right == nullptr)
                {
                    // Found a leaf node, so write the symbol to the output file
                    fwrite(&node->symbol, sizeof(uint8_t), 1, outputFile);

                    // Reset the Huffman tree traversal to the root node
                    node = root;
                    broken = true;
                }
            }
        }
    }

    // Close the output file
    fclose(outputFile);
}

void fDecompress(std::string &path, void (*fDisplayProgress)(int))
{
    // Step 1: Create the root and onpen the file
    HuffmanTreeNode *root = new HuffmanTreeNode('\0', 0);
    FILE *f = fopen(path.c_str(), "r+b");

    if (!f)
    {
        std::cerr << "Failed to open file for reading." << std::endl;
        return;
    }

    fDisplayProgress(25); // Step 1 of 4

    // Step 2: Rebuild the tree
    fRebuildTree(f, root);

    fDisplayProgress(50); // Step 2 of 4

    // Step 3: Read encoded info and decode into a new file
    fReadDecodeCreate(f, root, path);
    free(root);

    fDisplayProgress(75); // Step 3 of 4

    // Step 4: Safely close the file
    fclose(f);

    fDisplayProgress(100); // Step 4 of 4
}

#endif
