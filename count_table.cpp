
#include "count_table.hpp"

using namespace std;

CountTable::CountTable(const string fn):fileName(fn)
{
  // FIX Delete Char Map
}

CountTable::~CountTable()
{
  if ( !isPtr.is_open()) isPtr.close();
}

void CountTable::createCharMap()
{
  unsigned char c;

  if ( !isPtr.is_open()) {
    isPtr.open(fileName.c_str());
    if (!isPtr) {
	cerr << "Error: Could not open: '" << fileName << "' for reading" << endl;
	return;
    }
  }
  while (isPtr.good()) {
      c = isPtr.get();
      if (isPtr.eof())
	break;
      add(c);
      count++;
    }
  add(EOF); // Add 'EOF' last so it gets the longest Seq

  if (isPtr.is_open()) isPtr.close();

} // End createCharMap()

void CountTable::add(unsigned char c)
{
  map<unsigned char,unsigned>::iterator it;

  it = ct.find(c);
  // If this char is already in the map, increment the count
  // otherwise add it to the map
  if ( it == ct.end()) 
    ct.insert( pair<unsigned char, unsigned>(c,1));
  else
    it->second += 1;

} // End CountTable::add()

void CountTable::printCharMap()
{
  cout << "########## Print Char Map ##########\n";
  unsigned i     = 0;
  unsigned count = 0;
  for( map<unsigned char,unsigned>::iterator ii=ct.begin(); ii!=ct.end(); ++ii) {
    if (ii->first > ' ' && ii->first < '\x7f' )
      printf("%2c\t%u\n", ii->first, ii->second);
    else
      printf("'%02x'\t%u\n", ii->first, ii->second);
    count += ii->second;
    i++;
  }
  cout << dec << "Number of different chars  : " << i << endl;
  cout << dec << "Total Number of chars read : " << count << endl;
}

// Create the Nodes 
// Create Huffman NodeList
void CountTable::buildNodeList(HuffmanTree& ht)
{
  for( map<unsigned char,unsigned>::iterator ii=ct.begin(); ii!=ct.end(); ++ii)
    ht.buildNodeList(ii->first, ii->second);

} // End buildNodeList()
