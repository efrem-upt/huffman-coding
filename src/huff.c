#include "resources/huffer.c"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Not enough arguments for the program to function\n");
        fprintf(stderr, "To run the program, the first parameter must be either the word <<enc>> or <<dec>>, and the second argument a path to a file.\n");
        fprintf(stderr, "Entering less or more than 2 arguments will not work");
        return 0;
    }

    if (strcmp(argv[1], "enc") == 0) {
        createCompressedFile(argv[2]);
        printf("Success. Encrypted given file.");
    }
    else if (strcmp(argv[1], "dec") == 0) {
        decryptCompressedFile(argv[2]);
        printf("Success. Decrypted given file.");
    }
    else fprintf(stderr, "Wrong command for first parameter. The first parameter must be either the word <<enc>> or the word <<dec>>");
    freeTree(HuffmanRoot);

    return 0;
}
