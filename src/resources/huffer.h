#include <stdio.h>
#define MAX_DATA 1000
#define MAX_HUFF_CODE 512

typedef struct KeyType {
    char content;
    int probability;
    int isSpecialNode;
}KeyType;

typedef struct Node {
    // node in Huffman/Binary tree
    KeyType key;
    struct Node* leftChild;
    struct Node* rightChild;
}Node;

typedef struct NodeData {
            // trees used as zone memory for storing partial results of Huffman trees
            Node* node;
        }NodeData;
NodeData data[MAX_DATA];

Node* HuffmanRoot = NULL; // root of the Huffman tree
Node* KeysRoot = NULL; // root of the binary tree containing Huffman trees
char HuffmanCodes[256][MAX_HUFF_CODE] = {}; // contains the Huffman codes for each character in the file
char FileTextToHuffman[MAX_HUFF_CODE*MAX_HUFF_CODE] = {}; // stores the text of the file encoded in the Huffman format
char HuffmanTreeEncryption[512] = {}; // stores the encryption for the Huffman tree so the decryption can be implemented
int numberOfData = 0;
void freeTree(Node* tree); // frees all the tree memory created using malloc
Node* addKeyToBinaryTree(char key); // adds the specified key in the binary tree containing all the keys and returns the node created
void removeKeyFromBinaryTree(char key); // removes the specified key in the binary tree containing all the keys
Node* getSmallestKey(Node* tree); // gets the key with the smallest probability from the binary tree
NodeData getSmallestData(); // gets the iteration with the smallest sum of probabilities from the IterationTrees array
void addData(Node* addedNode); // adds the iteration to the data array
void removeData(Node* searchedNode); // removes the iteration from the data array
Node* parseKeysTree(); // parses the keys in the binary tree and returns the corresponding Huffman Tree
void parseKeysFile(char* pathToFile); // parses the file and creates the binary tree containing the keys
void getHuffmanCodes(Node* tree, char buf[256]); // extracts the Huffman codes for each character in the Huffman tree (leaf nodes)
void transformFileTextToHuffman(char* pathToFile); // reads a file and transforms the file text in it's Huffman binary representation
unsigned char byteStringToBinaryChar(char* binaryString); // converts the binary string (8 bits) stored as a char array into a char variable that has the same binary representation
void charToBinaryString(unsigned char ch, char buf[9]); // converts a char to it's binary representation (8 bits)
void getHuffmanTreeEncryptionPrefix(Node* tree, char buf[MAX_HUFF_CODE*MAX_HUFF_CODE]); // used in Huffman file encryption algorithm (for the header)
void getHuffmanTreeEncryptionPostfix(Node* tree, char buf[MAX_HUFF_CODE*MAX_HUFF_CODE]); // used in Huffman file encryption algorithm (for the header)
void createCompressedFile(char* pathToFile); // creates the compressed file (in Huffman format), according to the file pointed by pathToFile
Node* createHuffmanTreeFromPrefix(char prefix[MAX_HUFF_CODE*MAX_HUFF_CODE]); // creates the Huffman tree format from the encrypted form (the Huffman tree prefix)
void updateHuffmanTreeFromPostfix(Node* tree, char postfix[MAX_HUFF_CODE*MAX_HUFF_CODE]); // updates the Huffman tree generated from the prefix with the actual content of the keys stored in the format postfix
void recoverHuffmanTree(char* pathToCompressedFile); // uses the prefix and postfix format of the Huffman tree, encrypted in the pathToCompressedFile specified, and with it builds a new tree in memory, that has the same prefix and postfix form as the one indicated by the compressed file
void decryptCompressedFile(char* pathToCompressedFile); // implemented decryption of file encoded in Huffman algorithm
void freeHuffmanMemory(Node* tree); // frees the memory of the Huffman tree
