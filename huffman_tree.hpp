#pragma once 

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <list>

using namespace std;

// Global Data Structures, Global Variables and Functions

struct BITS {
unsigned b1: 1;
unsigned b2: 1;
unsigned b3: 1;
unsigned b4: 1;
unsigned b5: 1;
unsigned b6: 1;
unsigned b7: 1;
unsigned b8: 1;
};

typedef enum BIT { ZERO=0, ONE=1, ENDOFFILE } Bit;

typedef struct HT* htPtr;
struct HT {
  unsigned char c;
  unsigned      weight;
  htPtr         zero;
  htPtr         one;
};

class HuffmanTree
{
private:
  htPtr       treeTop;
  htPtr       zero, one;
  htPtr       nodeArray[256];
  unsigned    nodeCount;
  htPtr       tZero, tOne;
  htPtr       getPtr;
  list<htPtr> nodeList;

  map<unsigned char, string> charSeqTable;

  void printNode(htPtr ptr, int level, char x, vector<string>* buffer);

public:
  HuffmanTree();
  void          buildNodeList(unsigned char, unsigned);
  void          printNodeList();
  void          writeNodeList(string&);
  void          buildTree();
  void          printTree();
  void          getSeq(htPtr ptr, Bit b, string s);
  void          genCharSeqTable();
  void          printCharSeqTable();
  string        getBitSeq(unsigned char c);
  unsigned char getChar(Bit);

}; // End Class HuffmanTree
