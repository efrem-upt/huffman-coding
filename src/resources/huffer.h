#include <stdio.h>
#define MAX_DATA 1000

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
            int isBinaryTreeNode;
        }NodeData;
NodeData data[MAX_DATA];

Node* HuffmanRoot; // root of the Huffman tree
Node* KeysRoot; // root of the binary tree containing Huffman trees
int numberOfData = 0;
void freeTree(Node* tree); // frees all the tree memory created using malloc
Node* addKeyToBinaryTree(char key); // adds the specified key in the binary tree containing all the keys and returns the node created
void removeKeyFromBinaryTree(char key); // removes the specified key in the binary tree containing all the keys
Node* getSmallestKey(Node* tree); // gets the key with the smallest probability from the binary tree
NodeData getSmallestData(); // gets the iteration with the smallest sum of probabilities from the IterationTrees array
void addData(Node* addedNode, int isBinaryTreeNode); // adds the iteration to the data array
void removeData(Node* searchedNode); // removes the iteration from the data array
Node* parseKeysTree(); // parses the keys in the binary tree and returns the corresponding Huffman Tree
void parseKeysFile(FILE* file); // parses the file and creates the binary tree containing the keys
