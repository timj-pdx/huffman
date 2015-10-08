#include <sstream>

#include <strings.h>
#include <vector>

#include "huffman_tree.hpp"

using namespace std;

// Global Data Structures, Global Variables and Functions

class UncompressFile {
private :
  unsigned char* readBuf;
  ifstream       isPtr;
  ofstream       osPtr;
  unsigned       bitCount;
  unsigned       byteCount;
  unsigned       bufCount;
  bool           endOfFile;
  const string   inFile;
  const string   outFile;
  const int      readBufSize;
  struct BITS*   bits;
public:
  UncompressFile(string, string);
  ~UncompressFile();
  Bit getBit();
  void printCompressedBits();
  void buildNodeList(HuffmanTree&);
  void doit(HuffmanTree&);

}; // End Class UncompressFile
