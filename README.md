# Huffman Encoder-Decoder System
## Description
This project is a compression program designed to convert letters into binary code based on a Huffman Tree. The Huffman Tree is established using a preliminarily given word-frequency list.
The program takes a user-defined string as input and print according binary code as output.
The program compresses the given text by converting each 4 digits of the according binary code into each bit of an integer number. By doing this, size of the primary text can be shorten.
The program will also be able to decode the text after compressing using the same Huffman Tree, which ensures the accuracy and uniqueness of the encode-decoder process.

## Functions
Functions of the system include:
* Establish a Huffman Tree using a preliminarily given word-frequency list
* Print the Huffman Tree
* Convert the user-defined string into a binary code
* Compress the binary code and shorten the text length
* Decode the encoded text

## Building Environment
The project has been tested using Xcode 11.3.1 on macOS Catalina 10.15.2

## Appendix
### Word-frequency list (%):
Space: 186  A: 64  B: 13  C: 22  D: 32  E: 103  F: 21  G: 15  H: 47  I: 57  J: 1  K: 5  L: 32  M: 20  N: 57  O: 63  P: 15  Q: 1  R: 48  S: 51  T: 80  U: 23  V: 8  W: 18  X: 1  Y: 16  Z: 1
