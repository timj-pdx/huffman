#include <strings.h>
#include "huffman_tree.hpp"
#include "compress_file.hpp"
#include "uncompress_file.hpp"
#include "count_table.hpp"

int main( int argc, char* argv[] )
{
  if ( argc != 2 ) {
    cerr << "\n    usage: " << argv[0] << " file\n\n";
    return 1;
  }
  string exec = argv[0];
  string fileName = argv[1];
  bool   compressFlag;

  exec.replace(0,exec.find_last_of('/')+1,""); // Get 'basename' of arg[0]
  if (exec == "compress") compressFlag = true;
  else                    compressFlag = false;

  if (compressFlag) {	// Compress

    string compressedFile = fileName + ".hc";

    CountTable  ct(fileName.c_str());
    HuffmanTree ht;

    ct.createCharMap();
    //ct.printCharMap();
    ct.buildNodeList(ht);
    //ht.printNodeList();
    ht.buildTree();
    //ht.printTree();
    ht.genCharSeqTable();
    //ht.printCharSeqTable();

    CompressFile cf(fileName, compressedFile);
    //ht.printNodeList();
    cf.doit(ht);

  } else {		// Uncompress

    string compressedFile   = fileName + ".hc";
    string uncompressedFile = fileName + ".uc";

    //  CountTable  ct(fileName);
    HuffmanTree ht;
    UncompressFile uf(compressedFile, uncompressedFile);

    uf.buildNodeList(ht);
    //ht.printNodeList();
    ht.buildTree();
    //ht.printTree();
    //uf.printCompressedBits();
    uf.doit(ht);

 }

} // End Main()
