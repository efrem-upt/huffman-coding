#include "huffer.h"
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define EOF 255

int dimFile = 0;

void freeTree(Node* tree) {
    if(tree) {
        struct Node* leftChild = tree->leftChild; // gets the left child
        struct Node* rightChild = tree->rightChild; // gets the right child
        free(tree); // frees the current
        freeTree(leftChild); // applies freeTree on left child
        freeTree(rightChild); // applies freeTree on right child
    }
}


Node* addKeyToBinaryTree(unsigned char key) {
    if (!KeysRoot) { // if there is no key in the tree
        // create the tree with one node
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
    // else add the node accordingly
    Node* pred = NULL;
    Node* iterator = KeysRoot;
    while (iterator != NULL) { // while we haven't found the position to insert
        pred = iterator;
        if (key < iterator->key.content)
            iterator = iterator->leftChild;
        else if (key == iterator->key.content) {
            iterator->key.probability++;
            return iterator;
        } else iterator = iterator->rightChild;
    }
    // create new node and associate the relations
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

void removeKeyFromBinaryTree(unsigned char key) {
    if (KeysRoot == NULL) // if there is no node in the tree
        return;
    Node* iterator = KeysRoot;
    Node* pred = NULL;
    while (iterator != NULL && iterator->key.content != key) { // while we haven't found the key
        if (iterator->key.content > key) { // if the key is smaller than current then go left
            pred = iterator;
            iterator = iterator->leftChild;
        }
        else if (iterator->key.content < key) { // else go right
            pred = iterator;
            iterator = iterator->rightChild;
        }
    }
    if (iterator != NULL) { // if we found the key
        if (!iterator->leftChild && !iterator->rightChild) { // if the node is leaf
            if (pred == NULL) { // if there is no node besides it (the tree contains only one node)
                KeysRoot = NULL;
                free(iterator);
                return;
            } // else remake the relations
                free(iterator);
                if (key < pred->key.content)
                    pred->leftChild = NULL;
                else pred->rightChild = NULL;
            } else if (iterator->leftChild && iterator->rightChild) { // if the node has both left and right child
                // find the biggest element on the left
                Node *leftIterator = iterator->leftChild;
                Node *leftPredIterator = NULL;
                while (leftIterator != NULL) {
                    leftPredIterator = leftIterator;
                    leftIterator = leftIterator->rightChild;
                }
                // switch biggest element with current and remove the node that contained the biggest element
                char newKey = leftPredIterator->key.content;
                unsigned long long newProbability = leftPredIterator->key.probability;
                removeKeyFromBinaryTree(newKey);
                iterator->key.content = newKey;
                iterator->key.probability = newProbability;
            } else { // it has only one child
                if (iterator->leftChild) { // it has left child
                    // switch keys and probabilities
                    iterator->key.content = iterator->leftChild->key.content;
                    iterator->key.probability = iterator->leftChild->key.probability;
                    // remake relations
                    Node* leftChild = iterator->leftChild->leftChild;
                    Node* rightChild = iterator->leftChild->rightChild;
                    free(iterator->leftChild);
                    iterator->leftChild = leftChild;
                    iterator->rightChild = rightChild;

                } else { // it has right child
                    // switch keys and probabilities
                    iterator->key.content = iterator->rightChild->key.content;
                    iterator->key.probability = iterator->rightChild->key.probability;
                    // remake relations
                    Node* leftChild = iterator->rightChild->leftChild;
                    Node* rightChild = iterator->rightChild->rightChild;
                    free(iterator->rightChild);
                    iterator->leftChild = leftChild;
                    iterator->rightChild = rightChild;
                }
            }
        }
}

void addData(Node* addedNode) {
    if (addedNode == NULL) // if node is null then we don't add it
        return;
    // add data and icnrement numberOfData
    data[numberOfData] = addedNode;
    numberOfData++;
}

void removeData(Node* searchedNode) {
    for (unsigned long long i = 0; i < numberOfData; i++) {
        if (data[i] == searchedNode) { // if node is found
            for (unsigned long long j = i; j < numberOfData; j++)
                data[j] = data[j + 1]; // move elements to the left
            numberOfData--; // decrement numberOfData
            break;
        }
    }
}

Node* getSmallestData() {
    Node* smallestData = data[0];
    unsigned long long smallestDataProbability; // for finding minimum probability
    if (smallestData)
        smallestDataProbability = data[0]->key.probability;

    for (unsigned long long i = 1; i < numberOfData; i++) {
        if (data[i]->key.probability < smallestDataProbability) { // if current key probability is smaller than the one we have
            smallestDataProbability = data[i]->key.probability; // update smallest probability
            smallestData = data[i]; // update smallest node found
        }
    }
    return smallestData;
}

Node* parseKeysTree() {
    if (KeysRoot == NULL)
        return NULL;
    else if (!KeysRoot->leftChild && !KeysRoot->rightChild) { // if tree contains only one node
        // creates the Huffman tree containing the copy of the same node
         HuffmanRoot = (Node* )malloc(sizeof(Node));
         HuffmanRoot->key.content = KeysRoot->key.content;
         HuffmanRoot->leftChild = NULL;
         HuffmanRoot->rightChild = NULL;
         removeKeyFromBinaryTree(KeysRoot->key.content);
         return HuffmanRoot; // exit function
    }
    while (KeysRoot) { // as long as we have nodes
        Node* auxNode = KeysRoot; // get the root of the keys tree
        // create copy of the node and add copy to data
        Node* copyNode = (Node* )malloc(sizeof(Node));
        copyNode->key.content = auxNode->key.content;
        copyNode->key.probability = auxNode->key.probability;
        copyNode->leftChild = NULL;
        copyNode->rightChild = NULL;
        addData(copyNode);
        removeKeyFromBinaryTree(auxNode->key.content); // removes the current root from the binary tree (refresh the binary tree)
    }
    do { // as long as we have data nodes
        Node* smallestNode = getSmallestData(); // get smallest data
        removeData(smallestNode);
        Node* secondSmallestNode = getSmallestData(); // get second smallest data
        removeData(secondSmallestNode);
        // create new node containing the two nodes as childs and probabilities the sum of the child probabilities
        Node* newRoot = (Node *)malloc(sizeof(Node));
        newRoot->key.content = '*';
        newRoot->leftChild = smallestNode;
        newRoot->rightChild = secondSmallestNode;
        unsigned long long leftProbability = (newRoot->leftChild) ? newRoot->leftChild->key.probability : 0;
        unsigned long long rightProbability = (newRoot->rightChild) ? newRoot->rightChild->key.probability : 0;
        newRoot->key.probability = leftProbability + rightProbability;
        HuffmanRoot = newRoot; // update the HuffmanRoot
        addData(HuffmanRoot); // adds the new node to data to be used in the selection process
    }while(numberOfData > 1);

    return HuffmanRoot;
}

int fileIterator = 0, endOfFileIndicator = 0;

FILE* parseKeysFile(FILE* file) {
    if (!file) {
        fprintf(stderr, "Error on opening the specified file\n");
        exit(EXIT_FAILURE);
    }

    unsigned char ch;
    int i = 0;
    do { // read charactes from the file and add them to the binary tree containing keys
        ch = fgetc(file);
        fileIterator++;
        if (ch != EOF) {
            addKeyToBinaryTree(ch);
            numberOfCharacters++;
        } else endOfFileIndicator = 1;
    }while(ch != EOF && fileIterator < MAX_FILE_ITERATOR);
    parseKeysTree(); // parse the binary tree and get Huffman tree
    return file;
}

void getHuffmanCodes(Node* tree, char buf[MAX_HUFF_CODE]) {
    if (!tree->leftChild && !tree->rightChild) { // if we found leaf node
        strcpy(HuffmanCodes[tree->key.content], buf); // copy current traversal to the HuffmanCodes array, that will contain the code for the current character
    }
    else {
        char aux1[MAX_HUFF_CODE] = {}; // auxiliary arrays to store traversals
        char aux2[MAX_HUFF_CODE] = {};
        strcpy(aux1, buf);
        strcpy(aux2, buf);
        getHuffmanCodes(tree->leftChild, strcat(aux1, "0")); // left = 0
        getHuffmanCodes(tree->rightChild, strcat(aux2, "1")); // right = 1
    }
}

void computeHuffmanCodesLength() {
    for (int i = 0; i < 256; i++) {
        int l = strlen(HuffmanCodes[i]);
        if (l)
            lengthOfHuffmanCodes[i] = l;
    }
}

FILE* transformFileTextToHuffman(FILE* file) {
    if (!file) {
        fprintf(stderr, "Error on opening the file.\n");
        exit(EXIT_FAILURE);
    }

    char ch[2] = {};
    int currentSizeOfHuffmanText = 0; // stores the length of the variable FileTextToHuffman, for storage analysis and "file is too large" function
    do { // read the file and add code representations from HuffmanCodes to the FileTextToHuffman by appending
        ch[0] = fgetc(file);
        fileIterator++;
        currentSizeOfHuffmanText += lengthOfHuffmanCodes[ch[0]];
        strcat(FileTextToHuffman, HuffmanCodes[ch[0]]);
    }while(ch[0] != EOF && fileIterator < MAX_FILE_ITERATOR);
    return file;
}

unsigned char byteStringToBinaryChar(char* binaryString) {
    unsigned char value = 0;
    unsigned char exp = 1;
    for (int i = strlen(binaryString) - 1; i >= 0; i--) { // approach like in mathematics: if abcd is a binary string than it's decimal representation is d*2^0 + c*2^1 + b*2^2 + a*2^3
        value = value + (binaryString[i] - '0')*exp;
        exp*=2;
    }
    return value;
}

void charToBinaryString(unsigned char ch, char buf[9]) {
    for (int i = 0; i < 8; i++) // initialize buffer
        buf[i] = '0';
    buf[8] = '\0';
    // exp - stores the smallest power of 2 bigger than the character
    unsigned char exp = 0;
    unsigned char aux = ch;
    while(aux) {
        aux = aux/2;
        if(aux)
            exp++;
    }
    unsigned char old_exp = exp;
    while(ch) { // right-left approach until the variable becomes 0
        if (ch % 2 == 0) // if character is divisible by 2
            buf[7 - (old_exp-exp)] = '0'; // then add a 0
        else buf[7 - (old_exp-exp)] = '1'; // else add a 1
        exp--; // decrease exp to iterate through all characters in buffer
        ch = ch/2; // to calculate the next binary digit
    }

}

// Several notions on how I have encrypted the Huffman tree:
// For the encryption of the actual tree structure, I do a preorder traversal on the Huffman tree
// in that traversal, I mark with 0 non leaf-nodes and with 1 leaf-nodes. This is what I call the prefix of the Huffman tree encryption.
// For example, let the following Huffman tree:
//              *
//           *     C
//        A    B
// The prefix of the tree stated is as follows: 00111 (5 bits). Thus, the prefix of a Huffman tree with n nodes has n bits.
// To reconstruct the tree from it's prefix, we simply go as follows:
// the first 0 tells us that there is no leaf-node, so we mark that node with * and go to the left (because that's what we did to encrypt it)
// the next 0 tells us that there is no leaf-node, we mark it with * and go to the left
// the next 1 tells us that there is a leaf-node, we mark it with <blank> and go to the right of it's parent
// the next 1 tells us that there is a leaf-node, we mark it with <blank> and because we've exhaused the parent nodes from left to right, we go to the parent's parent's right
// the next 1 tells us that there is a leaf-node, so we mark it with <blank> and because there is no bit after it, the tree has been reconstructed
// After reconstructing, the tree looks like this:
//              *
//           *     <blank>
//   <blank>    <blank>
// The problem is that we don't have access to the real keys the old tree had. This is where the notion of postfix comes from.
// The postfix stores the actual representation of the keys stored in the leaf nodes, from left to right.
// For instance, take the tree we used to have:
//              *
//           *     C
//        A    B
// We apply the same preorder algorithm on this tree and, once we get to a leaf node, we store the binary representation of it's key
// First is A, then is B, and then is C, and their binary representations are 01000001, 01000010, 01000011.
// So, the postfix will be: 010000010100001001000011. We can notice the postfix has a length of 8*numberOfKeysInHuffmanTree
// To reconstruct the tree and get the same nodes as in the original, we do a preorder traversal on the tree obtained after prefix reconstruction
// Once we get to a leaf node that is currently blank, we convert the first chunk of 8 bits in the postfix and assign that key in the tree with the character representation
// Doing this for all leaf-nodes, the original Huffman tree is reconstructed
// So, the encoding for the original Huffman tree above, combining the prefix and postfix, is as follows: 00111010000010100001001000011, which is 29 bits, or 4 bytes.

// Because computers work better in multiples of eight, in my solution I've done the following: I have extended the prefix length to the smallest number of eight bigger or equal than the current length.
// For example, take our prefix: 00111. Because it's on 5 bits, I am extending it to 8 bits by adding 3 zeros to the left: 00000111.
// Doing this, operations will be simpler to make, as the whole encryption will have it's length a multiple of eight.
// For this, 2 bytes have to be used: 1 byte to store the length of the fixed prefix (the one multiple of eight), and 1 byte to store the length of the actual prefix (the one without the extra zeroes at the beginning)
// These 2 bytes will be part of the file header, along with the 4 bytes of the tree encoding above.
// An extra 4 bytes of the hader will be used to store the number of encoded Huffman keys after parsing the file. Thus, the total header size for this tree is 4 + 2 + 4 = 10 bytes. The same header will be used for all combinations
// of the characters A,B,C, no matter the length of the string.

void getHuffmanTreeEncryptionPrefix(Node* tree, char buf[PREFIX_LENGTH]) {
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

void getHuffmanTreeEncryptionPostfix(Node* tree, char buf[POSTFIX_LENGTH]) {
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

void init() {
    data[0] = NULL;
    numberOfData = 0;
    numberOfCharacters = 0;
    freeTree(HuffmanRoot);
    strcpy(FileTextToHuffman, "");
    strcpy(HuffmanTreeEncryption, "");
    strcpy(encryption, "");
    fileIterator = 0;
}

void createCompressedFile(char* pathToFile) {
    char filePath[FILE_PATH_LENGTH] = {};
    strcpy(filePath, pathToFile);
    char* fileExtension = strchr(filePath, '.') + 1;
    strcpy(fileExtension, "EfremHuffmanCompression");
    FILE* compressedFile = fopen(filePath,"wb");
        if (!compressedFile) {
            fprintf(stderr,"Error on creating compressed file");
            freeTree(HuffmanRoot);
            exit(EXIT_FAILURE);
        }
    FILE* file1 = fopen(pathToFile, "r");
    FILE* file2 = fopen(pathToFile, "r");
    FILE* file3 = fopen(pathToFile, "r");
    fseek(file3, 0, SEEK_END);
    dimFile = ftell(file3);
    fclose(file3);
    int i = 0;
    while (endOfFileIndicator == 0) {
        init();
        printf("Parsing file...\n");
        parseKeysFile(file1);
        fileIterator = 0;
        char buf[MAX_HUFF_CODE] = {};
        printf("Getting Huffman codes...\n");
        getHuffmanCodes(HuffmanRoot, buf);
        computeHuffmanCodesLength();
        printf("Getting the text's Huffman representation...\n");
        transformFileTextToHuffman(file2);
        char prefix[PREFIX_LENGTH] = {};
        char postfix[POSTFIX_LENGTH] = {};
        printf("Encoding the Huffman tree...\n");
        getHuffmanTreeEncryptionPrefix(HuffmanRoot, prefix);
        getHuffmanTreeEncryptionPostfix(HuffmanRoot, postfix);
        unsigned char characters_to_read = strlen(prefix);
        unsigned char biggestNumberOfEightButSmallest = 0;
        while (biggestNumberOfEightButSmallest < characters_to_read)
            biggestNumberOfEightButSmallest += 8;
        char* fixed_prefix = (char*)calloc(biggestNumberOfEightButSmallest + 1, sizeof(char));
        if (!fixed_prefix) {
            fprintf(stderr, "Memory error on fixing the prefix");
            freeTree(HuffmanRoot);
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < biggestNumberOfEightButSmallest; i++)
            fixed_prefix[i] = '0';
        for (int i = 0; i < characters_to_read; i++) {
            fixed_prefix[biggestNumberOfEightButSmallest-1-i] = prefix[characters_to_read-1-i];
        }
        fixed_prefix[biggestNumberOfEightButSmallest] = '\0';
        strcat(HuffmanTreeEncryption, fixed_prefix);
        free(fixed_prefix);
        strcat(HuffmanTreeEncryption, postfix);
        printf("Writing header to compressed file...\n");
        fwrite(&characters_to_read,sizeof(unsigned char),1,compressedFile);
        fwrite(&biggestNumberOfEightButSmallest, sizeof(unsigned char), 1 , compressedFile);
        fwrite(&numberOfCharacters, sizeof(unsigned int), 1, compressedFile);
        char aux1[PREFIX_LENGTH + POSTFIX_LENGTH] = {};
        strcpy(aux1,HuffmanTreeEncryption);
        int dim1 = strlen(aux1);
        char aux2[MAX_FILE_ITERATOR*MAX_HUFF_CODE] = {};
        strcpy(aux2,FileTextToHuffman);
        int dim2 = strlen(aux2);
        fwrite(&dim1,sizeof(int), 1, compressedFile);
        fwrite(&dim2,sizeof(int), 1, compressedFile);
        while(dim1) {
            char aux_byte[9] = {};
            strncpy(aux_byte,aux1,8);
            unsigned char generated_byte = byteStringToBinaryChar(aux_byte);
            fwrite(&generated_byte,sizeof(unsigned char),1,compressedFile);
            if(dim1 >= 8) {
                strcpy(aux1,aux1+8);
                dim1 -= 8;
            }
            else {
                strcpy(aux1,aux1+dim1);
                dim1 -= dim1;
            }
        }
        printf("Writing Huffman encoded text to compressed file...\n");
        while(dim2) {
            char aux_byte[9] = {};
            strncpy(aux_byte,aux2,8);
            if (dim2 < 8) {
                for (int i = dim2; i < 8; i++)
                    aux_byte[i] = '0';
                aux_byte[8] = '\0';
                dim2 = 8;
            }
            unsigned char generated_byte = byteStringToBinaryChar(aux_byte);
            fwrite(&generated_byte,sizeof(unsigned char),1,compressedFile);
            strcpy(aux2,aux2+8);
            dim2 -= 8;
        }
    }
    freeTree(HuffmanRoot);
    fclose(compressedFile);
}

Node* createHuffmanTreeFromPrefix(char prefix[PREFIX_LENGTH]) {
        if (!strlen(prefix))
            return NULL;
        Node* tree = (Node*)malloc(sizeof(Node));
        if (!tree) {
            fprintf(stderr,"Memory error on creating Huffman tree from prefix");
            freeTree(HuffmanRoot);
            exit(EXIT_FAILURE);
        }
        if (prefix[0] == '1') {
            tree->leftChild = NULL;
            tree->rightChild = NULL;
        } else {
            tree->key.content = '*';
            tree->leftChild = createHuffmanTreeFromPrefix(strcpy(prefix,prefix+1));
            tree->rightChild = createHuffmanTreeFromPrefix(strcpy(prefix,prefix+1));
        }
        return tree;
}

void updateHuffmanTreeFromPostfix(Node* tree, char postfix[POSTFIX_LENGTH]) {
    if (!tree->leftChild && !tree->rightChild) {
        char aux_byte[9] = {};
        strncpy(aux_byte,postfix,8);
        if(strlen(postfix) >= 8)
            strcpy(postfix, postfix+8);
        else strcpy(postfix, postfix+strlen(postfix));
        char ch = byteStringToBinaryChar(aux_byte);
        tree->key.content = ch;
    } else {
        updateHuffmanTreeFromPostfix(tree->leftChild, postfix);
        updateHuffmanTreeFromPostfix(tree->rightChild, postfix);
    }
}

int dim1 = 0;
int dim2 = 0;
int auxDim2 = 0;

void recoverHuffmanTree(FILE* compressedFile) {

    if (!compressedFile) {
        fprintf(stderr,"Error on reading the compressed file");
        freeTree(HuffmanRoot);
        exit(EXIT_FAILURE);
    }

    unsigned char characters_to_read = 0;
    unsigned char correct_characters = 0;
    fread(&characters_to_read,sizeof(unsigned char), 1, compressedFile);
    fread(&correct_characters,sizeof(unsigned char), 1, compressedFile);
    fread(&numberOfCharacters,sizeof(unsigned int), 1, compressedFile);
    if (numberOfCharacters == 0)
        return;
    fread(&dim1,sizeof(int), 1, compressedFile);
    fread(&dim2,sizeof(int), 1, compressedFile);
    auxDim2 = dim2;
    unsigned char aux = 0;
    while(dim1 + dim2 > 0 && fread(&aux,sizeof(unsigned char), 1, compressedFile) == 1) {
        char binary_representation[9] = {};
        charToBinaryString(aux, binary_representation);
        strcat(encryption, binary_representation);
        if (dim1 > 0)
            dim1 -= 8;
        else if (dim2 > 0)
            dim2 -= 8;
    }
    char prefix[PREFIX_LENGTH] = {};
    strncpy(prefix, encryption, correct_characters);
    char* fixed_prefix = prefix + (correct_characters - characters_to_read);
    int number_of_ones = 0;
    int length_prefix = strlen(fixed_prefix);
    for (int i = 0; i < length_prefix; i++) {
        if (fixed_prefix[i] == '1')
            number_of_ones++;
    }
    strcpy(encryption, encryption + correct_characters);
    char postfix[POSTFIX_LENGTH] = {};
    strncpy(postfix, encryption, number_of_ones*8);
    strcpy(encryption, encryption + number_of_ones*8);
    HuffmanRoot = createHuffmanTreeFromPrefix(fixed_prefix);
    updateHuffmanTreeFromPostfix(HuffmanRoot, postfix);
}

void decryptCompressedFile(char* pathToCompressedFile) {
    FILE* compressedFile = fopen(pathToCompressedFile, "rb");
    char filePath[FILE_PATH_LENGTH] = {};
    strcpy(filePath, pathToCompressedFile);
    char* fileExtension = strchr(filePath, '.');
    strcpy(fileExtension, "_decrypted.txt");
    FILE* uncompressedFile = fopen(filePath, "w");
    if (!uncompressedFile) {
            fprintf(stderr, "Couldn't create the uncompressed file with fopen");
            freeTree(HuffmanRoot);
            exit(EXIT_FAILURE);
        }

    while(!feof(compressedFile)) {
        printf("Recovering the Huffman tree from the header...\n");
        strcpy(encryption,"");
        recoverHuffmanTree(compressedFile);
        Node* iterator = HuffmanRoot;
        if (iterator == NULL)
            break;
        printf("Decrypting the Huffman encoded text into the decrypted file...\n");
        while(numberOfCharacters) {
            if (!iterator->leftChild && !iterator->rightChild && numberOfCharacters) {
                fprintf(uncompressedFile, "%c", iterator->key.content);
                iterator = HuffmanRoot;
                numberOfCharacters--;
            }
            if (encryption[0] == '0')
                iterator = iterator->leftChild;
            else iterator = iterator->rightChild;
            strcpy(encryption, encryption + 1);
        }
        if (!iterator->leftChild && !iterator->rightChild && numberOfCharacters)
            fprintf(uncompressedFile, "%c", iterator->key.content);
        freeTree(HuffmanRoot);
        HuffmanRoot = NULL;
    }
    fclose(compressedFile);
    fclose(uncompressedFile);
}


