#include "resources/huffer.c"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        printHelp();
        return 0;
    }
    encryption = (char*)calloc((PREFIX_LENGTH+POSTFIX_LENGTH + MAX_FILE_ITERATOR*MAX_HUFF_CODE + 1), sizeof(char));
    FileTextToHuffman = (char*)calloc((MAX_FILE_ITERATOR*MAX_HUFF_CODE + 1), sizeof(char));
    char *encryptionAux = encryption;
    if (strcmp(argv[1], "--enc") == 0 && argc == 3) {
        createCompressedFile(argv[2]);
        printf("\nSuccess. Encoded given file.\n");
    }
    else if (strcmp(argv[1], "--dec") == 0 && argc == 3) {
        decryptCompressedFile(argv[2]);
        printf("\nSuccess. Decoded given file.\n");
    }
    else if (strcmp(argv[1], "--help") == 0) {
        printHelp();
    }
    else fprintf(stderr, "Wrong commands. Use --help to view options.\n");
    free(encryptionAux);
    free(FileTextToHuffman);
    return 0;
}
