# Huffman Coding App
A desktop app to demonstrate Huffman Coding, a lossless data compression algorithm. Files can be compressed and decompressed within the GUI. Large text files can typically be compressed by around 30 - 40%, or about 60 - 70% of their original size, and then decompressed without any loss of data. The algorithm itself is written in C++, and the GUI is written in Python.

![Screenshot](Huffman.PNG)

## Compiling

To run this project, you will need to compile huffman.cpp with the following command:

`g++ -shared -o libhuffman.dll huffman.cpp`
