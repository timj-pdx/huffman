SHELL = bash

all: compress uncompress ascii

compress: main.o count_table.o huffman_tree.o compress_file.o uncompress_file.o
	g++ $^ -o $@

uncompress:
	ln -s compress uncompress

# This executable will create a test file that contains every ascii character
# In this case the compressed file is larger than the uncompressed version
ascii:ascii.o
	g++ $^ -o $@

.cpp.o:
	g++ -Wall -g -c $*.cpp -o $@

testit:
	cat *.hpp *.cpp > data
	./compress data
	./uncompress data
	if diff data data.uc ; then \
		echo OK; \
	else \
		echo BAD; \
	fi

clean:
	rm -f *.o compress ascii uncompress *~

count_table.o:     count_table.cpp count_table.hpp
compress_file.o:   compress_file.cpp compress_file.hpp huffman_tree.hpp
uncompress_file.o: uncompress_file.cpp uncompress_file.hpp huffman_tree.hpp
huffman_tree.o:    huffman_tree.cpp huffman_tree.hpp
main.o:            main.cpp count_table.hpp huffman_tree.hpp compress_file.hpp
