#include "huffman_tree.hpp"

void HuffmanTree::printNode(htPtr ptr, int level, char side, vector<string>* buffer)
{
    char tmpBuf[1024];

    if ( ptr == NULL ) return;
    if ( ptr->c == '\0' ) {
      sprintf(tmpBuf, " (%c      %2d ) ", side, ptr->weight);
      (*buffer)[level] += tmpBuf;
    } else {
      sprintf(tmpBuf, " (%c '%02x' %2d ) ", side, ptr->c, ptr->weight);
      (*buffer)[level] += tmpBuf;
    }
    printNode( ptr->zero, level+1, '0', buffer);
    printNode( ptr->one,  level+1, '1', buffer);

} // End printNode

HuffmanTree::HuffmanTree()
{
  nodeCount = 0;
  tZero     = NULL;
  tOne      = NULL;
  getPtr    = NULL;
  treeTop   = NULL;
};

void HuffmanTree::buildNodeList(unsigned char c, unsigned weight)
{
  htPtr ptr = new struct HT;
  ptr->c      = c;
  ptr->weight = weight;
  ptr->zero   = NULL;
  ptr->one    = NULL;
  nodeList.push_back(ptr);

} // End buildArray()

static bool sortHT (htPtr first, htPtr second) { return first->weight>second->weight; }

void HuffmanTree::printNodeList()
{
  cout << "########## Print Node List ##########\n";
  list<htPtr>::iterator it;

  for (it=nodeList.begin(); it!=nodeList.end(); ++it)
    if ( (*it)->c > ' ' && (*it)->c < '\x7f' )
      printf("%2c\t%u\n", (*it)->c, (*it)->weight);
    else
      printf("'%02x'\t%u\n", (*it)->c, (*it)->weight);

} // End printNodeList()

// Write the Node List pairs to the first line of the compressed file
// When uncompress runs it will use these pairs to generate a new 'CharMap'
void HuffmanTree::writeNodeList(string& header)
{
  char   buf[64];

  header.erase();
  list<htPtr>::iterator it;
  for (it=nodeList.begin(); it!=nodeList.end(); ++it) {
    sprintf(buf, "%x %u ", (*it)->c, (*it)->weight);
    header += buf;
  }

} // End writeNodeList()

void HuffmanTree::buildTree()
{
  tZero = tOne = NULL;
  list<htPtr> tmpList;

  list<htPtr>::iterator it;
  for (it=nodeList.begin(); it!=nodeList.end(); ++it)
    tmpList.push_front((*it));
  tmpList.sort(sortHT);

  while (tmpList.size() > 1) {
    htPtr tmpZero = tmpList.back();
    htPtr tmpOne;
    tmpList.pop_back();
    if (tmpList.empty()) {
      tmpOne = NULL;
    } else {
      tmpOne = tmpList.back();
      tmpList.pop_back();
    }
    htPtr tmp = new struct HT;
    tmp->zero = tmpZero;
    tmp->one  = tmpOne;
    tmp->c    = '\0';
    if (tmpOne == NULL )
      tmp->weight = tmp->zero->weight;
    else
      tmp->weight = tmp->zero->weight + tmp->one->weight;
    tmpList.push_back(tmp);
    tmpList.sort(sortHT);
    //    for(list<htPtr>::iterator it=tmpList.begin(); it!=tmpList.end(); ++it)
    //      cout << (*it)->weight << endl;
  }
  treeTop = tmpList.back();

}	// End buildTree()

void HuffmanTree::printTree()
{
  vector <string> buffer(128);
  printNode( treeTop, 0, 't', &buffer );
  cout << "########## Print Tree ################" << endl;
  for (int i=0; !buffer[i].empty(); i++) {
    printf( "Level %d %s\n", i, buffer[i].c_str());
  }
} // End printTree()

void HuffmanTree::getSeq(htPtr ptr, Bit b, string s)
{
  if ( ptr == NULL ) return;
  if (b == ZERO) s += "0";
  if (b == ONE)  s += "1";
  if (ptr->c == '\0' ) {
    getSeq(ptr->zero, ZERO, s);
    getSeq(ptr->one,  ONE,  s);
  } else {
    charSeqTable[ptr->c] = s.c_str();
  }

} // End getSeq()

void HuffmanTree::genCharSeqTable()
{
  htPtr ptr;
  ptr = treeTop;
  getSeq(ptr->zero, ZERO, "");
  getSeq(ptr->one,  ONE,  "");
}

void HuffmanTree::printCharSeqTable()
{
  cout << "########## Print Char Sequence Table ##########\n";
  for( map<unsigned char,string>::iterator ii=charSeqTable.begin(); ii!=charSeqTable.end(); ++ii) {
    if (ii->first > ' ' && ii->first < '\x7f' )
      printf(" %c\t%s\n", ii->first, ii->second.c_str());
    else
      printf("'%02x'\t%s\n", ii->first, ii->second.c_str());
  }
}

string HuffmanTree::getBitSeq(unsigned char c)  { return charSeqTable[c]; }

// getChar
// Descend the tree each time getChar() is called. If a leaf is not reached
// return '\xf0'. When a leave node is found, return the character.
// If the leaf node does not contain a char, then return '\xf1' which means
// sequence does map to a character.

unsigned char HuffmanTree::getChar(Bit bit)
{

  if (getPtr == NULL) getPtr = treeTop;
  if (bit == ONE) getPtr = getPtr->one;
  else            getPtr = getPtr->zero;
  if (getPtr == NULL) {
    cerr << "getChar(): Internal Error : Sequence not found\n";
    return '\xf1';
  }
  if (getPtr->c == '\0') return '\xf0';
  unsigned char ret = getPtr->c;
  getPtr = treeTop;
  return ret;

} // End getChar()
