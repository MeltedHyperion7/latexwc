latexwc: latexwc.c file_funcs.o walk_file.o bracket_stack.o tag_array.o
	gcc -w -o build/$@ $< build/file_funcs.o build/walk_file.o build/bracket_stack.o build/tag_array.o

bracket_stack.o: bracket_stack.c bracket_stack.h
	gcc -w -c -o build/$@ $<

bracket_stack_test: bracket_stack_test.c bracket_stack.o
	gcc -w -o test/$@ $< build/bracket_stack.o

file_funcs.o: file_funcs.c file_funcs.h bracket_stack.o
	gcc -w -c -o build/$@ $< build/bracket_stack.o

walk_file.o: walk_file.c walk_file.h bracket_stack.o
	gcc -w -c -o build/$@ $< build/bracket_stack.o

tag_array.o: tag_array.c tag_array.h
	gcc -w -c -o build/$@ $<

latexwc_test: latexwc.c file_funcs.o walk_file.o bracket_stack.o tag_array.o
	gcc -w -g -O0 -o test/$@ $< build/file_funcs.o build/walk_file.o build/bracket_stack.o build/tag_array.o
	test/latexwc_test test/test1.tex
