#include <stdio.h>

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

KeyType keys[256];
Node* HuffmanRoot; // root of the Huffman tree
Node* KeysRoot; // root of the binary tree containing Huffman trees
void freeTree(Node* tree); // frees all the tree memory created using malloc
void addKeyToBinaryTree(char key); // adds the specified key in the binary tree containing all the keys
void removeKeyFromBinaryTree(char key); // removes the specified key in the binary tree containing all the keys
Node* parseKeysTree(KeyType keys[]); // parses the keys in the binary tree and returns the corresponding Huffman Tree
void parseKeysFile(FILE* file); // parses the file and creates the binary tree containing the keys
