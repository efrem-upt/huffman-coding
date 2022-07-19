#include "resources/huffer.c"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Not enough arguments for the program to function\n");
        fprintf(stderr, "To run the program, the first parameter must be either the word <<enc>> or <<dec>>, and the second argument a path to a file.\n");
        fprintf(stderr, "Entering less or more than 2 arguments will not work");
        return 0;
    }
    encryption = (char*)calloc((PREFIX_LENGTH+POSTFIX_LENGTH + MAX_FILE_ITERATOR*MAX_HUFF_CODE), sizeof(char));
    FileTextToHuffman = (char*)calloc((MAX_FILE_ITERATOR*MAX_HUFF_CODE), sizeof(char));
    if (strcmp(argv[1], "enc") == 0) {
        createCompressedFile(argv[2]);
        printf("\nSuccess. Encrypted given file.");
    }
    else if (strcmp(argv[1], "dec") == 0) {
        decryptCompressedFile(argv[2]);
        printf("\nSuccess. Decrypted given file.");
    }
    else fprintf(stderr, "Wrong command for first parameter. The first parameter must be either the word <<enc>> or the word <<dec>>");

    free(encryption);
    free(FileTextToHuffman);
    return 0;
}
