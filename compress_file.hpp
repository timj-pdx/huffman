#pragma once

#include <string.h>

#include "huffman_tree.hpp"

// Global Data Structures, Global Variables and Functions

class CompressFile {
private:
  unsigned char Buf;
  unsigned int  bitCount;
  ofstream      osPtr;
  ifstream      isPtr;
  string        inFileName;
  string        outFileName;
  string        endOfFileSeq;
  
  void pushBit(const unsigned char);
  void pushBits(const char*);
  void close();

public:
  CompressFile(string, string);
  ~CompressFile();
  void doit(HuffmanTree&);

}; // End Class CompressFile
