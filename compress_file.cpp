
#include "compress_file.hpp"

// Global Data Structures, Global Variables and Functions

void CompressFile::pushBit(const unsigned char c)
{
  if (bitCount == 8) {
    // printf("pb %x\n", Buf);
    osPtr.put(Buf);
    bitCount = 0;
    Buf = '\0';
  }
  //cout << c << endl;
  Buf = Buf << 1;
  if (c == '1') Buf++;
  bitCount++;

} // End pushBit()

void CompressFile::pushBits(const char* str)
{

  for( unsigned i=0; i<strlen(str); i++)
    pushBit(str[i]);

} // End pushBits()

CompressFile::CompressFile(string ifn, string ofn):inFileName(ifn),outFileName(ofn)
{
  Buf = '\0';
  bitCount = 0;
} // End CompressFile (Constructor)

CompressFile::~CompressFile()
{
  if (osPtr.is_open()) osPtr.close();
  if (isPtr.is_open()) isPtr.close();

} // End CompressFile (Destructor)

void CompressFile::close()
{
  // Flush 'Buf'
  if (bitCount < 8) Buf = Buf << (8-bitCount);
  //printf("~pb %x\n", (unsigned)Buf);
  osPtr.put(Buf);
  osPtr.close();

} // End close()

void CompressFile::doit(HuffmanTree& ht)
 {
  if (!isPtr.is_open()) {     // Open file for Reading
    isPtr.open(inFileName.c_str());
    if (!isPtr) {
      cerr << "Error: Could not open: '" << inFileName << "' for reading" << endl;
      return;
    }
  }

  if (!osPtr.is_open()) {     // Open file for Writing
    osPtr.open(outFileName.c_str());
    if (!osPtr) {
      cerr << "Error: Could not open: '" << outFileName << "' for writing\n";
      return;
    }
 }
  // Write head to first line in uncompressed file.
  string header;
  ht.writeNodeList(header);
  osPtr << header << endl;

  // Read from original file
  // Write compressed bits to 'compressed' file
  while (isPtr.good()) {
    unsigned char c = isPtr.get();
    if (isPtr.eof()) {
      pushBits(ht.getBitSeq(EOF).c_str());
      break;
    }
    pushBits(ht.getBitSeq(c).c_str());
  }
  if (isPtr.is_open()) isPtr.close();
  close();

} // End CompressFile::doit()

