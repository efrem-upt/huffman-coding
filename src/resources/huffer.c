#include "huffer.h"
#include <stdlib.h>

void freeTree(Node* tree) {
    // elibereaza memoria arborelui in caz de probleme la malloc
    if(tree) {
        struct Node* leftChild = tree->leftChild;
        struct Node* rightChild = tree->rightChild;
        free(tree);
        freeTree(leftChild);
        freeTree(rightChild);
    }
}


void addKeyToBinaryTree(char key) {
    if (!KeysRoot) {
        KeysRoot = (Node*)malloc(sizeof(Node));
        if (!KeysRoot) {
            fprintf(stderr,"\nmalloc error on root initialisation\n");
            exit(EXIT_FAILURE);
        }
        KeysRoot->key.content = key;
        KeysRoot->key.probability++;
        KeysRoot->leftChild = NULL;
        KeysRoot->rightChild = NULL;
        return;
    }

    Node* pred = NULL;
    Node* iterator = KeysRoot;
    while (iterator != NULL) {
        pred = iterator;
        if (key < iterator->key.content)
            iterator = iterator->leftChild;
        else if (key == iterator->key.content) {
            iterator->key.probability++;
            return;
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
            if (iterator->key.probability > 1) {
                iterator->key.probability--;
                return;
            }
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

void preordine(Node* referinta) {
    // parcurgerea in preordine in arbore binar ordonat
    if (referinta) {
        printf("%c ", referinta->key.content);
        preordine(referinta->leftChild);
        preordine(referinta->rightChild);
    }
}

