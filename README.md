# huffman-coding
C program that allows for encryption/decryption of ASCII text files into and from the Huffman coding algorithm. Small project made during my second year of Computer Engineering studies at Polytechnic University of Timișoara

## Running the program

The program can only be run via the command line.

To run the program, simply choose from the following two actions:
 - encrypt a text file (ASCII characters only) on your system using the Huffman compression format
 - decrypt a file on your system that has already been encrypted by my program

For instance, supposing you have a file test.txt on your system, located in the same fodler you have the executable on, then to encrypt the file the following command can be executed:
```bash
  huff enc test.txt
```

After this, a new file will be generated, having the same name as the one given as second parameter to the program but with the extension .EfremHuffmanCompression. To decrypt this file, the following command can be executed:
```bash
  huff dec test.EfremHuffmanCompression
```
Thus, the file generated will have the same name as the file you are trying to decrypt, but with "_decryped" appended to it. To view the decrypted file, simply access test_decrypted.txt on your system. The file should have the same content as the text.txt file given in the first step.
## Demo

![hippo](https://lh3.googleusercontent.com/fife/AAWUweWt3Nl6Qelrbvv_lclqvRu2Ohq3nBRwJX1EM6kJ5DENkCWW8HnthohxqJlPs0VcB59ewWLoIFPWF4PTGruaqvddD-sImEGoyAxCrsdBezfgaSrBQ8jSHurhZkTLhLwqW6raoO5U96xJcigiEIPnRrsMqsLjfo1J-3rXmZkO6LRMr5VKv4rSOCW1nB4sKO5usMhJ-VWSBx-vvAEQrJzwua8RxQh3EdC11GIWXU7OHA1NacFM2vINzZmLoBnE-zNU2ReuSWi3HTsUENHpmUxPaUpDk_Hqn0wIpDSu5cR63yuX4TUMe8qQMZedPNmCbbBnzQjpsfmPgmMMERIk-qrLOxX_sPr2R76ptJu7Sv7A_gYUH--rZn4gB0yOxV4gNXYFyH_X-LUCdNKBsMt-Ui5SBDoopZorSp0Q1dI9GXKAWfZniGWRohQnCtcPbVLYXlHgx3nxMRSVe8nB6scrJlEtu4FfdVV5PlfOzOzpsj1L-IN0mshHIpSLMRUam3QTxvaG6Gn-Mph__MYBzAhimnQ2VZdMHjFrAP936G583R7fwYj1BNnko7qhnhUBk8v-KpzuDWFL_HKO2yP3OCYVKm7QBp2xTyrcKbmZaaweBtdgdCh_Jg5midE1gNBnPu1xewwc0nw8oUawu82a45rflvEgczQ3GtaXMRiXBmdq38JvFnew6sJfh5f4c7GyLmznPUrAV2y1ggID5vryrJYZqrRD-Rw851xJBuEQgGdB3i6PxmiZa1cmB7ZNwsmck7phGMNAR6FFMlMSt3iEyXskXCw=w1920-h932)
## License

[GNU GPLv3](https://choosealicense.com/licenses/gpl-3.0/)

