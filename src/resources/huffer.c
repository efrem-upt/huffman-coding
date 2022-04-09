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
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;

    if (key < pred->key.content)
        pred->leftChild = newNode;
    else
        pred->rightChild = newNode;
}
