#include "huffer.h"
#include <stdlib.h>
#include <string.h>
#define MAX_PROB 0xFFFF

void freeTree(Node* tree) {
    if(tree) {
        struct Node* leftChild = tree->leftChild;
        struct Node* rightChild = tree->rightChild;
        free(tree);
        freeTree(leftChild);
        freeTree(rightChild);
    }
}


Node* addKeyToBinaryTree(char key) {
    if (!KeysRoot) {
        KeysRoot = (Node*)malloc(sizeof(Node));
        if (!KeysRoot) {
            fprintf(stderr,"\nmalloc error on binary tree root initialisation\n");
            exit(EXIT_FAILURE);
        }
        KeysRoot->key.content = key;
        KeysRoot->key.probability++;
        KeysRoot->leftChild = NULL;
        KeysRoot->rightChild = NULL;
        return KeysRoot;
    }

    Node* pred = NULL;
    Node* iterator = KeysRoot;
    while (iterator != NULL) {
        pred = iterator;
        if (key < iterator->key.content)
            iterator = iterator->leftChild;
        else if (key == iterator->key.content) {
            iterator->key.probability++;
            return iterator;
        } else iterator = iterator->rightChild;
    }

    Node* newNode = (Node*)malloc(sizeof(struct Node));
    if (!newNode) {
        fprintf(stderr,"\nmalloc error on adding a new node");
        freeTree(KeysRoot);
        exit(EXIT_FAILURE);
    }
    newNode->key.content = key;
    newNode->key.probability = 1;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;

    if (key < pred->key.content)
        pred->leftChild = newNode;
    else
        pred->rightChild = newNode;
    return newNode;
}

void removeKeyFromBinaryTree(char key) {
    if (KeysRoot == NULL)
        return;
    Node* iterator = KeysRoot;
    Node* pred = NULL;
    while (iterator != NULL) {
        if (iterator->key.content > key) {
            pred = iterator;
            iterator = iterator->leftChild;
        }
        else if (iterator->key.content < key) {
            pred = iterator;
            iterator = iterator->rightChild;
        }
        else {
            if (!iterator->leftChild && !iterator->rightChild) {
                if (pred == NULL) {
                    KeysRoot = NULL;
                    return;
                }
                free(iterator);
                if (key < pred->key.content)
                    pred->leftChild = NULL;
                else pred->rightChild = NULL;
            } else if (iterator->leftChild && iterator->rightChild) {
                Node *leftIterator = iterator->leftChild;
                Node *leftPredIterator = NULL;
                while (leftIterator != NULL) {
                    leftPredIterator = leftIterator;
                    leftIterator = leftIterator->rightChild;
                }
                char newKey = leftPredIterator->key.content;
                int newProbability = leftPredIterator->key.probability;
                removeKeyFromBinaryTree(newKey);
                iterator->key.content = newKey;
                iterator->key.probability = newProbability;
            } else {
                if (iterator->leftChild) {
                    iterator->key.content = iterator->leftChild->key.content;
                    iterator->key.probability = iterator->leftChild->key.probability;
                    Node* leftChild = iterator->leftChild->leftChild;
                    Node* rightChild = iterator->leftChild->rightChild;
                    free(iterator->leftChild);
                    iterator->leftChild = leftChild;
                    iterator->rightChild = rightChild;

                } else {
                    iterator->key.content = iterator->rightChild->key.content;
                    iterator->key.probability = iterator->rightChild->key.probability;
                    Node* leftChild = iterator->rightChild->leftChild;
                    Node* rightChild = iterator->rightChild->rightChild;
                    free(iterator->rightChild);
                    iterator->leftChild = leftChild;
                    iterator->rightChild = rightChild;
                }
            }
        }
    }
}

Node* getSmallestKey(Node* tree) {
    if (tree) {
        Node* leftTree = getSmallestKey(tree->leftChild);
        Node* rightTree = getSmallestKey(tree->rightChild);
        int leftProbability = (leftTree != NULL) ? (leftTree->key.probability) : MAX_PROB;
        int rightProbability = (rightTree != NULL) ? (rightTree->key.probability) : MAX_PROB;
        if (tree->key.probability < rightProbability) {
            if (tree->key.probability < leftProbability)
                return tree;
            else return leftTree;
        } else if (leftProbability <= rightProbability)
            return leftTree;
          else return rightTree;
    } else return NULL;


}

void addData(Node* addedNode) {
    if (addedNode == NULL)
        return;
    data[numberOfData].node = addedNode;
    numberOfData++;
}

void removeData(Node* searchedNode) {
    for (int i = 0; i < numberOfData; i++) {
        if (data[i].node == searchedNode) {
            for (int j = i; j < numberOfData; j++)
                data[j] = data[j + 1];
            numberOfData--;
            break;
        }
    }
}

NodeData getSmallestData() {
    int smallestDataProbability = MAX_PROB;
    NodeData smallestData;
    smallestData.node = NULL;
    for (int i = 0; i < numberOfData; i++) {
        if (data[i].node->key.probability < smallestDataProbability) {
            smallestDataProbability = data[i].node->key.probability;
            smallestData = data[i];
        }
    }
    return smallestData;
}

Node* parseKeysTree() {
    if (KeysRoot == NULL)
        return NULL;
    else if (!KeysRoot->leftChild && !KeysRoot->rightChild) {
         HuffmanRoot = (Node* )malloc(sizeof(Node));
         HuffmanRoot->key.content = KeysRoot->key.content;
         HuffmanRoot->key.isSpecialNode = 0;
         HuffmanRoot->leftChild = NULL;
         HuffmanRoot->rightChild = NULL;
         return HuffmanRoot;
    }
    while (KeysRoot) {
        Node* smallestNode = getSmallestKey(KeysRoot);
        Node* copyNode = (Node* )malloc(sizeof(Node));
        copyNode->key.content = smallestNode->key.content;
        copyNode->key.probability = smallestNode->key.probability;
        copyNode->leftChild = NULL;
        copyNode->rightChild = NULL;
        addData(copyNode);
        removeKeyFromBinaryTree(smallestNode->key.content);
    }
    do {
        NodeData smallestNode = getSmallestData();
        removeData(smallestNode.node);
        NodeData secondSmallestNode = getSmallestData();
        removeData(secondSmallestNode.node);
        Node* newRoot = (Node *)malloc(sizeof(Node));
        newRoot->key.content = '*';
        newRoot->key.isSpecialNode = 1;
        newRoot->leftChild = smallestNode.node;
        newRoot->rightChild = secondSmallestNode.node;
        int leftProbability = (newRoot->leftChild) ? newRoot->leftChild->key.probability : 0;
        int rightProbability = (newRoot->rightChild) ? newRoot->rightChild->key.probability : 0;
        newRoot->key.probability = leftProbability + rightProbability;
        HuffmanRoot = newRoot;
        addData(HuffmanRoot);
    }while(numberOfData > 1);

    return HuffmanRoot;
}

void parseKeysFile(char* pathLoFile) {
    FILE* file = fopen(pathLoFile, "r");
    if (!file) {
        fprintf(stderr, "Eroare la citirea fisierului dat\n");
        exit(EXIT_FAILURE);
    }

    char ch;
    do {
        ch = fgetc(file);
        if (ch != EOF)
            addKeyToBinaryTree(ch);
    }while(ch != EOF);
    fclose(file);
    parseKeysTree();
}

void getHuffmanCodes(Node* tree, char buf[MAX_HUFF_CODE]) {
    if (!tree->leftChild && !tree->rightChild)
        strcpy(HuffmanCodes[tree->key.content], buf);
    else {
        char aux1[256] = {};
        char aux2[256] = {};
        strcpy(aux1, buf);
        strcpy(aux2, buf);
        getHuffmanCodes(tree->leftChild, strcat(aux1, "0"));
        getHuffmanCodes(tree->rightChild, strcat(aux2, "1"));
    }
}

void transformFileTextToHuffman(char* pathToFile) {
    FILE* file = fopen(pathToFile, "r");
    if (!file) {
        fprintf(stderr, "Eroare la citirea fisierului dat\n");
        exit(EXIT_FAILURE);
    }

    char ch[2] = {};
    do {
        ch[0] = fgetc(file);
        strcat(FileTextToHuffman, HuffmanCodes[ch[0]]);
    }while(ch[0] != EOF);
    fclose(file);
}

unsigned char byteStringToBinaryChar(char* binaryString) {
    unsigned char value = 0;
    unsigned char exp = 1;
    for (int i = strlen(binaryString) - 1; i >= 0; i--) {
        value = value + (binaryString[i] - '0')*exp;
        exp*=2;
    }
    return value;
}

void charToBinaryString(unsigned char ch, char buf[9]) {
    for (int i = 0; i < 8; i++)
        buf[i] = '0';
    buf[8] = '\0';
    unsigned char exp = 0;
    unsigned char aux = ch;
    while(aux) {
        aux = aux/2;
        if(aux)
            exp++;
    }
    unsigned char old_exp = exp;
    while(ch) {
        if (ch % 2 == 0)
            buf[7 - (old_exp-exp)] = '0';
        else buf[7 - (old_exp-exp)] = '1';
        exp--;
        ch = ch/2;
    }

}

void getHuffmanTreeEncryptionPrefix(Node* tree, char buf[MAX_HUFF_CODE*MAX_HUFF_CODE]) {
    if (tree == NULL)
        return;
    if (!tree->leftChild && !tree->rightChild)
        strcat(buf,"1");
    else {
        strcat(buf,"0");
        getHuffmanTreeEncryptionPrefix(tree->leftChild, buf);
        getHuffmanTreeEncryptionPrefix(tree->rightChild, buf);
    }
}

void getHuffmanTreeEncryptionPostfix(Node* tree, char buf[MAX_HUFF_CODE*MAX_HUFF_CODE]) {
    if (tree == NULL)
        return;
    if (!tree->leftChild && !tree->rightChild) {
        char char_representation[9];
        charToBinaryString(tree->key.content, char_representation);
        strcat(buf,char_representation);
    } else {
        getHuffmanTreeEncryptionPostfix(tree->leftChild, buf);
        getHuffmanTreeEncryptionPostfix(tree->rightChild, buf);
    }
}

void createCompressedFile(char* pathToFile) {
    parseKeysFile(pathToFile);
    char buf[MAX_HUFF_CODE] = {};
    getHuffmanCodes(HuffmanRoot, buf);
    transformFileTextToHuffman(pathToFile);
    char prefix[MAX_HUFF_CODE*MAX_HUFF_CODE] = {};
    char postfix[MAX_HUFF_CODE*MAX_HUFF_CODE] = {};
    getHuffmanTreeEncryptionPrefix(HuffmanRoot, prefix);
    getHuffmanTreeEncryptionPostfix(HuffmanRoot, postfix);
    char HuffmanTreeEncryption[2*MAX_HUFF_CODE*MAX_HUFF_CODE] = {};
    strcat(HuffmanTreeEncryption, prefix);
    strcat(HuffmanTreeEncryption, postfix);

    FILE* compressedFile = fopen("resources/file.compressed","wb");
    if (!compressedFile) {
        fprintf(stderr,"Error on creating compressed file");
        exit(EXIT_FAILURE);
    }

    char aux1[2*MAX_HUFF_CODE*MAX_HUFF_CODE] = {};
    strcpy(aux1,HuffmanTreeEncryption);

    while(strlen(aux1)) {
        char aux_byte[9] = {};
        strncpy(aux_byte,aux1,8);
        unsigned char generated_byte = byteStringToBinaryChar(aux_byte);
        fwrite(&generated_byte,sizeof(unsigned char),1,compressedFile);
        if(strlen(aux1) >= 8)
            strcpy(aux1,aux1+8);
        else strcpy(aux1,aux1+strlen(aux1));
    }

    char aux2[MAX_HUFF_CODE*MAX_HUFF_CODE] = {};
    strcpy(aux2,FileTextToHuffman);

    while(strlen(aux2)) {
        char aux_byte[9] = {};
        strncpy(aux_byte,aux2,8);
        unsigned char generated_byte = byteStringToBinaryChar(aux_byte);
        fwrite(&generated_byte,sizeof(unsigned char),1,compressedFile);
        if(strlen(aux2) >= 8)
            strcpy(aux2,aux2+8);
        else strcpy(aux2,aux2+strlen(aux2));
    }
    fclose(compressedFile);
}

void viewTree(Node* tree) {
    if (tree) {
        viewTree(tree->leftChild);
        if (tree->key.content == ' ')
            printf("blank ");
        else
            printf("%c ", tree->key.content);
        viewTree(tree->rightChild);
    }
}
