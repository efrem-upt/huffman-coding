#include "huffer.h"
#include <stdlib.h>
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
                removeKeyFromBinaryTree(newKey);
                iterator->key.content = newKey;
            } else {
                if (iterator->leftChild) {
                    iterator->key.content = iterator->leftChild->key.content;
                    Node* leftChild = iterator->leftChild->leftChild;
                    Node* rightChild = iterator->leftChild->rightChild;
                    free(iterator->leftChild);
                    iterator->leftChild = leftChild;
                    iterator->rightChild = rightChild;

                } else {
                    iterator->key.content = iterator->rightChild->key.content;
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

void addData(Node* addedNode, int isBinaryTreeNode) {
    if (addedNode == NULL)
        return;
    data[numberOfData].node = addedNode;
    data[numberOfData].isBinaryTreeNode = isBinaryTreeNode;
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
    smallestData.isBinaryTreeNode = 0;
    for (int i = 0; i < numberOfData; i++) {
        if (data[i].node->key.probability < smallestDataProbability) {
            smallestDataProbability = data[i].node->key.probability;
            smallestData = data[i];
        }
    }
    return smallestData;
}

Node* parseKeysTree() {

    do {
        Node* smallestKey = getSmallestKey(KeysRoot);
        if (smallestKey)
            removeKeyFromBinaryTree(smallestKey->key.content);
        Node* secondSmallestKey = getSmallestKey(KeysRoot);
        if (secondSmallestKey)
            removeKeyFromBinaryTree(secondSmallestKey->key.content);
        addData(smallestKey, 1);
        addData(secondSmallestKey, 1);
        NodeData smallestNode = getSmallestData();
        removeData(smallestNode.node);
        NodeData secondSmallestNode = getSmallestData();
        removeData(secondSmallestNode.node);
        Node* newRoot = (Node *)malloc(sizeof(Node));
        newRoot->key.isSpecialNode = 1;
        newRoot->leftChild = smallestNode.node;
        newRoot->rightChild = secondSmallestNode.node;
        int leftProbability = (newRoot->leftChild) ? newRoot->leftChild->key.probability : 0;
        int rightProbability = (newRoot->rightChild) ? newRoot->rightChild->key.probability : 0;
        newRoot->key.probability = leftProbability + rightProbability;
        HuffmanRoot = newRoot;
        addData(HuffmanRoot, 0);
        for (int i = 0; i < 2; i++) {
            if (numberOfData - 2 - i < 0)
                break;
            NodeData aux = data[numberOfData-2-i];
            if (aux.isBinaryTreeNode == 1) {
                addKeyToBinaryTree(aux.node->key.content);
                removeData(aux.node);
            }
        }

    }while (KeysRoot != NULL);

    return HuffmanRoot;
}

