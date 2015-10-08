#include "uncompress_file.hpp"

UncompressFile::UncompressFile(string in, string out):inFile(in),outFile(out),readBufSize(1024)
{
  bitCount  = 0;
  byteCount = 0;
  bufCount  = 0;
  endOfFile = false;
  readBuf   = new unsigned char[readBufSize];
}

UncompressFile::~UncompressFile()
{
  if ( isPtr.is_open()) isPtr.close();
  if ( osPtr.is_open()) osPtr.close();
  delete readBuf;
}

Bit UncompressFile::getBit()
{
  if (!isPtr.is_open()) {     // Open file
    isPtr.open(inFile.c_str());
    if (!isPtr) {
      cerr << "Error: Could not open: '" << inFile << "' for reading" << endl;
      return ENDOFFILE;
    }
    isPtr.getline((char*)readBuf, readBufSize);
    bitCount  = 0;
    byteCount = 0;
    bufCount  = 0;
    endOfFile = false;
  }

  if (byteCount ==  0 && bitCount == 0) { // Read from file into 'readBuf'
    int i;

    if ( endOfFile ) return ENDOFFILE;
    for (i=0; i<readBufSize; i++) {
      if (!isPtr.good()) {
	cerr << "ERROR : Reading '" << inFile << "'\n";
	break;
      }
      readBuf[i] = isPtr.get();
      byteCount = i;
      if (isPtr.eof()) {
	endOfFile = true;
	break;
      }
    }
    byteCount = i;
    readBuf[byteCount] = '\0';
    bitCount = 0;
    bufCount = 0;
  }
  if (bitCount == 0) { // Move 'Bit' pointer to next char
    //      printf("%02x ", readBuf[bufCount]);
    bits = (BITS*) &(readBuf[bufCount]);
    bufCount++;
    byteCount--;
    bitCount = 8;
  }

  switch (bitCount--) {
  case 1: if (bits->b1 == 0) return ZERO; break;
  case 2: if (bits->b2 == 0) return ZERO; break;
  case 3: if (bits->b3 == 0) return ZERO; break;
  case 4: if (bits->b4 == 0) return ZERO; break;
  case 5: if (bits->b5 == 0) return ZERO; break;
  case 6: if (bits->b6 == 0) return ZERO; break;
  case 7: if (bits->b7 == 0) return ZERO; break;
  case 8: if (bits->b8 == 0) return ZERO; break;
  default: cerr << "Internal Error\n"; return ENDOFFILE;
  }
  return ONE;

} // End getBit()

void UncompressFile::printCompressedBits()
{
  Bit bit;
  unsigned i;

  cout << "########## Print Compressd Bits ##########\n";
  for (i=0; ; i++) {
    bit = getBit();
    if (bit == ENDOFFILE)
      break;
    cout << (unsigned)bit;
    if (i % 4 == 3) cout << " ";
    if (i % 8 == 7) cout << endl;
  }
  if ( isPtr.is_open()) isPtr.close();

} // End printCompressedBits()
 
void UncompressFile::buildNodeList(HuffmanTree& ht)
{
  if ( !isPtr.is_open()) {
    isPtr.open(inFile.c_str());
    if (!isPtr) {
      cerr << "Error: Could not open: '" << inFile << "' for reading" << endl;
      return;
    }
  }

  //  char readBuf[readBufSize];
  isPtr.getline((char*)readBuf,readBufSize);

  string buf; // Have a buffer string
  vector<string> token;
  stringstream ss((char*)readBuf);
  unsigned int count;

  // Tokenize the string into a vector
  while (ss >> buf) {
    token.push_back(buf);
    count++;
  }

  // Validate that we have a even number
  if (count % 2 != 0) {
    cerr << "CountTable::extractCharMap() : Internal Error\n";
    return;
  }

  unsigned int c;
  // Convert the first token to 'hex' and the second to 'unsigned'
  for (unsigned i=0; i<token.size(); i+=2) {
    istringstream hex_char(token[i]);
    hex_char >> hex >> c;
    istringstream int_char(token[i+1]);
    int_char >> count;
    ht.buildNodeList(c, count);
  }

  if (isPtr.is_open()) isPtr.close();

} // End buildNodeList()

void UncompressFile::doit(HuffmanTree& ht)
{
  if (!osPtr.is_open()) {     // Open file
    osPtr.open(outFile.c_str());
    if (!osPtr) {
      cerr << "Error: Could not open: '" << outFile << "' for writing" << endl;
      return;
    }
  }

  unsigned char c;

  while (true) {
    c = ht.getChar(getBit());
    if (c < (unsigned char)'\x80') {
      //      if ( c  > ' ' ) printf("'%c'\n", c);
      //      else            printf("'%02x'\n", c);
      osPtr.put(c);
      continue;
    } else if (c == (unsigned char)EOF) {
      break;
    } else if (c == (unsigned char)'\xf0') { // Not there yet
      continue;
    } else if (c == (unsigned char)'\xf1') {
      cout << "uncompress() : Internal Error\n";
      break;
    }
  }
  if ( isPtr.is_open()) isPtr.close();
  if ( osPtr.is_open()) osPtr.close();

} // End doit()
