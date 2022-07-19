#include <stdio.h>
#define MAX_DATA 256
// the maximum number of nodes in the Huffman data creation process
#define MAX_HUFF_CODE 257
// the maximum length of a character's Huffman code, once it has been encrypted
#define MAX_FILE_ITERATOR 32000
// the number of data manipulated from the file at a given time
#define FILE_PATH_LENGTH 260
// the maximum length a file path given as argument can take
#define PREFIX_LENGTH 256 + 255 + 1
// the length the encoded Huffman tree prefix can take
#define POSTFIX_LENGTH 256*8 + 1 // the length the encoded Huffman tree postfix can take
// reasoning for choosing the PREFIX_LENGTH and POSTFIX_LENGTH: there can be a maximum of 256 different characters in a file (from the ASCII set), and so the Huffman tree created can have up to 255 special characters.
// Thus, the total number of nodes present in a Huffman tree with 256 different characters can be 256 + 255. For the prefix, 300 characters is enough for all scenarios. For the postfix, we store chunks of 8 bits for each character
// present in the Huffman tree. We can have a maximum number of 256 different characters (leaf nodes), so total size for postfix can be 256*8. Adding all up, the total size of the ecnryption of any Huffman tree
// with maximum 256 characters, using this format, is PREFIX_LENGTH + POSTFIX_LENGTH.


typedef struct KeyType {
    // Data Structure for the key
    unsigned char content;
    unsigned long long probability;
}KeyType;

typedef struct Node {
    // node in Huffman / Binary tree
    KeyType key;
    struct Node* leftChild;
    struct Node* rightChild;
}Node;

Node* data[MAX_DATA] = {}; // node array for storing nodes and partial Huffman trees during the Huffman tree creation process
unsigned numberOfData = 0; // initial count of data
unsigned short numberOfCharacters = 0; // stores the number of characters in the file
Node* HuffmanRoot = NULL; // root of the Huffman tree
Node* KeysRoot = NULL; // root of the binary tree containing keys
char HuffmanCodes[256][MAX_HUFF_CODE] = {}; // contains the Huffman codes for each character in the filehe le
unsigned lengthOfHuffmanCodes[256] = {}; // stores the length of all Huffman codes after computing them, used for "file is too large" feature
char* FileTextToHuffman = NULL; // stores the text of the file encoded in the Huffman format (text file must be <= MAX_FILE_SIZE bytes)
char HuffmanTreeEncryption[PREFIX_LENGTH+POSTFIX_LENGTH] = {}; // stores the encryption for the Huffman tree in prefix and postfix format so the recovery of the tree can be implemented
char* encryption = NULL; // used for decryption, stores all the bytes for both the Huffman tree encryption and the file text encryption
void freeTree(Node* tree); // frees all the tree memory created using malloc
Node* addKeyToBinaryTree(unsigned char key); // adds the specified key in the binary tree containing all the keys and returns the node created
void removeKeyFromBinaryTree(unsigned char key); // removes the specified key in the binary tree containing all the keys
Node* getSmallestData(); // gets the node with the smallest sum of probabilities from the data array
void addData(Node* addedNode); // adds the iteration to the data array
void removeData(Node* searchedNode); // removes the iteration from the data array
Node* parseKeysTree(); // parses the keys in the binary tree and returns the corresponding Huffman Tree
FILE* parseKeysFile(FILE* file); // parses the file and creates the binary tree containing the keys
void getHuffmanCodes(Node* tree, char buf[256]); // extracts the Huffman codes for each character in the Huffman tree (leaf nodes)
void computeHuffmanCodesLength(); // computes the length of all Huffman codes, used for the "file is too large" feature
FILE* transformFileTextToHuffman(FILE* file); // reads a file and transforms the file text in it's Huffman binary representation
unsigned char byteStringToBinaryChar(char* binaryString); // converts the binary string (string that contains ones and zeros) stored as a char array into a char variable that has the same binary representation (maximum 8 bits)
void charToBinaryString(unsigned char ch, char buf[9]); // converts a char to it's binary representation (8 bits)
void getHuffmanTreeEncryptionPrefix(Node* tree, char buf[PREFIX_LENGTH]); // used in Huffman file encryption algorithm (for the header)
void getHuffmanTreeEncryptionPostfix(Node* tree, char buf[POSTFIX_LENGTH]); // used in Huffman file encryption algorithm (for the header)
void createCompressedFile(char* pathToFile); // creates the compressed file (in Huffman format), according to the file pointed by pathToFile
Node* createHuffmanTreeFromPrefix(char prefix[PREFIX_LENGTH]); // creates the Huffman tree format from the encrypted form (the Huffman tree prefix)
void updateHuffmanTreeFromPostfix(Node* tree, char postfix[POSTFIX_LENGTH]); // updates the Huffman tree generated from the prefix with the actual content of the keys stored in the format postfix
void recoverHuffmanTree(FILE* compressedFile); // uses the prefix and postfix format of the Huffman tree, encrypted in the pathToCompressedFile specified, and with it builds a new tree in memory, that has the same prefix and postfix form as the one indicated by the compressed file
void decryptCompressedFile(char* pathToCompressedFile); // implemented decryption of file encoded in Huffman algorithm
