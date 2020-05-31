install: latexwc
	cp -r .latexwc ~/.latexwc
	cp build/latexwc /usr/bin/latexwc

latexwc: latexwc.c file_funcs.o walk_file.o bracket_stack.o tag_array.o cargs.o stopwords_tree.o
	gcc -w -o build/$@ $< build/file_funcs.o build/walk_file.o build/bracket_stack.o build/tag_array.o build/cargs.o build/stopwords_tree.o

cargs.o: cargs/cargs.c cargs/cargs.h
	gcc -w -c -o build/$@ $<

dcargs.o: cargs/cargs.c cargs/cargs.h
	gcc -g -w -c -o test/$@ $<

stopwords_tree.o: stopwords_tree.c stopwords_tree.h
	gcc -w -c -o build/$@ $<

dstopwords_tree.o: stopwords_tree.c stopwords_tree.h
	gcc -g -w -c -o test/$@ $<

bracket_stack.o: bracket_stack.c bracket_stack.h
	gcc -w -c -o build/$@ $<

dbracket_stack.o: bracket_stack.c bracket_stack.h
	gcc -g -w -c -o test/$@ $<

file_funcs.o: file_funcs.c file_funcs.h bracket_stack.o
	gcc -w -c -o build/$@ $< build/bracket_stack.o

dfile_funcs.o: file_funcs.c file_funcs.h dbracket_stack.o
	gcc -g -w -c -o test/$@ $< test/dbracket_stack.o

walk_file.o: walk_file.c walk_file.h bracket_stack.o
	gcc -w -c -o build/$@ $< build/bracket_stack.o

dwalk_file.o: walk_file.c walk_file.h dbracket_stack.o
	gcc -g -w -c -o test/$@ $< test/dbracket_stack.o

tag_array.o: tag_array.c tag_array.h
	gcc -w -c -o build/$@ $<

dtag_array.o: tag_array.c tag_array.h
	gcc -g -w -c -o test/$@ $<

latexwc_test: latexwc.c dfile_funcs.o dwalk_file.o dbracket_stack.o dtag_array.o dcargs.o dstopwords_tree.o
	gcc -w -g -o test/$@ $< test/dfile_funcs.o test/dwalk_file.o test/dbracket_stack.o test/dtag_array.o test/dcargs.o test/dstopwords_tree.o
	# ./test/latexwc_test ./test/test1.tex