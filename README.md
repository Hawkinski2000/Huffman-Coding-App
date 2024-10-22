# Huffman Coding App
A desktop app to demonstrate Huffman Coding, a lossless data compression algorithm. Files can be compressed and decompressed within the GUI. Written in C++ and Python. Large text files can typically be compressed by around 37%, or about 63% of their original size, and then decompressed without any loss of data.

![Screenshot](Huffman.PNG)

## Compiling

To run this project, you will need to compile huffman.cpp with the following command:

`g++ -shared -o libhuffman.dll huffman.cpp`
