#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <map>

#include "huffman_tree.hpp"

using namespace std;

typedef struct ENTRY* EntryPtr;
struct ENTRY {
  unsigned char c;
  ifstream      isPtr;
  EntryPtr      next;
  EntryPtr      prev;
};

class CountTable {
private:

  map<unsigned char, unsigned> ct;
  ifstream     isPtr;
  unsigned     count;		// The number of characters read from the file
  const string fileName;

public:
  unsigned     diffCount;	// The number of characters read from the file

  CountTable(const string filename);
  ~CountTable();
  void     createCharMap();          // Create charMap from original file
  void     add(unsigned char c);
  void     printCharMap();
  void     buildNodeList(HuffmanTree& ht);
  string   getEOF();

}; // End Class CountTable
