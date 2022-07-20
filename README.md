# huffman-coding

<p align="center">
  <img src="https://raw.githubusercontent.com/dragosefrem/huffman-coding/readme-changes/docs/icon.png" width = "200" height = "200">
</p>

C program (Windows only) that allows for encoding/decoding of binary files into and from the Huffman coding algorithm. Small project made for the Design and analysis of algorithms (RO) lab during my second year of Computer Engineering studies at Polytechnic University of Timișoara.
## Running the program (Windows)

The program can only be run via the command line.

To run the program, simply choose from the following two actions:
 - encode a binary file on your system using the Huffman compression format
 - decode a file on your system that has already been encrypted by my program

For instance, supposing you have a file test.txt on your system, located in the same folder you have the executable on, then to encode/compress the file the following command can be executed:
```bash
  huff --enc test.txt
```

After this, a new file will be generated, having the same name as the one given as second parameter to the program but with "_encoded" appended to it and with the extension .EfremHuffmanCompression. In this case, it's going to be test_encoded.EfremHuffmanCompression. To decode this file, the following command can be executed:
```bash
  huff --dec test.EfremHuffmanCompression
```
Thus, the file generated will have the same name as the file you are trying to decrypt, but with "_encoded" appended to it. To view the decrypted file, simply access test_encoded_decoded.txt on your system. The file should have the same content as the text.txt file given in the first step.

*Note: For options the user can enter the command `--help`.*
## Features

- Able to compress any type of binary file (image files, audio files, text files etc).
- Supports any file size.
- Fast and reliable compression and decompression.

*Note: It's possible not all binary files will compress to a smaller size. This is due to the uniformity of binary data being read and the probability of not existing patterns strong enough so that data is replaced with smaller data. For example, text files contain patterns, have many repeating symbols and the human language uses a limited set of characters, which means text files have a great ratio of being compressed to much smaller sizes then, let's say, image files, where data is much more arbitrary.*
## Documentation

Several notions on how I have encoded the Huffman tree:

 For the encoding of the actual tree structure, I do a preorder traversal on the Huffman tree in that traversal, I mark with 0 non leaf-nodes and with 1 leaf-nodes. This is what I call the `prefix` of the Huffman tree encryption.

 For example, let the following Huffman tree:

              *
           *     C
       A    B

The prefix of the tree stated is as follows: 00111 (5 bits). Thus, the prefix of a Huffman tree with n nodes has n bits.

To reconstruct the tree from it's prefix, we simply go as follows:

- the first 0 tells us that there is no leaf-node, so we mark that node with * and go to the left (because that's what we did to encrypt it)
- the next 0 tells us that there is no leaf-node, we mark it with * and go to the left
- the next 1 tells us that there is a leaf-node, we mark it with <blank> and go to the right of it's parent
- the next 1 tells us that there is a leaf-node, we mark it with <blank> and because we've exhaused the parent nodes from left to right, we go to the parent's parent's right
- the next 1 tells us that there is a leaf-node, so we mark it with <blank> and because there is no bit after it, the tree has been reconstructed

After reconstructing, the tree looks like this:

             *
           *     <blank>
    <blank>    <blank>

 The problem is that we don't have access to the real keys the old tree had. This is where the notion of `postfix` comes from.

The postfix stores the actual representation of the keys stored in the leaf nodes, from left to right.

For instance, take the tree we used to have:

              *
           *     C
        A    B

 We apply the same preorder algorithm on this tree and, once we get to a leaf node, we store the binary representation of it's key.

First is A, then is B, and then is C, and their binary representations are 01000001, 01000010, 01000011.

So, the postfix will be: 010000010100001001000011. We can notice the postfix has a length of 8*numberOfKeysInHuffmanTree.

To reconstruct the tree and get the same nodes as in the original, we do a preorder traversal on the tree obtained after prefix reconstruction.

Once we get to a leaf node that is currently blank, we convert the first chunk of 8 bits in the postfix and assign that key in the tree with the character representation.

Doing this for all leaf-nodes, the original Huffman tree is reconstructed.

Thus, the encoding for the original Huffman tree above, combining the prefix and postfix, is as follows: 00111010000010100001001000011, which is 29 bits, or 4 bytes.

For better operation, in my solution I've done the following: I have extended the prefix length to the smallest number of eight bigger or equal than the current length.

For example, take our prefix: 00111. Because it's on 5 bits, I am extending it to 8 bits by adding 3 zeros to the left: 00000111.

For this, 4 bytes have to be used: 2 bytes to store the length of the fixed prefix (the one multiple of eight), and 2 bytes to store the length of the actual prefix (the one without the extra zeroes at the beginning).

These 4 bytes will be part of the file header, along with the 4 bytes of the tree encoding above.

An extra 2 bytes of the header will be used to store the number of encoded Huffman keys after parsing the file. The header will also contain the length of the Huffman Tree Encryption (2 bytes), the length of the Huffman Text Encoding (4 bytes), and also the size of the file (8 bytes) and the file extension length (dynamic number of bytes, depending on how long the file extension is. Length n => n bytes), the latter two being used for decoding. The same header will be used for all combinations of the characters A,B,C, no matter the length of the string.

*Note: To improve the performance of the program I've made it so the program doesn't use the whole file at a time, but rather a portion of it (around 64 KB at a time). It compresses that portion and writes it to the compressed file. For each compressed portion, a small header will be used. There is a global header, indepedent of the iteration headers, consisted of the size of the file to be encoded and the length of the file extension.*
## Demo

<p align="center">
  <img src="https://github.com/dragosefrem/huffman-coding/blob/readme-changes/docs/Demo.gif" width = "800" height = "500">
</p>
## License

[GNU GPLv3](https://choosealicense.com/licenses/gpl-3.0/)

