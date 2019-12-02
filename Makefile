bracket_stack.o: bracket_stack.c bracket_stack.h
	gcc -c -o build/$@ $<

bracket_stack_test: bracket_stack_test.c bracket_stack.o
	gcc -o test/$@ $< build/bracket_stack.o

file_funcs.o: file_funcs.c file_funcs.h bracket_stack.o
	gcc -c -o build/$@ $< build/bracket_stack.o

walk_file.o: walk_file.c walk_file.h bracket_stack.o
	gcc -c -o build/$@ $< build/bracket_stack.o

tag_array.o: tag_array.c tag_array.h
	gcc -c -o build/$@ $<

latexwc_test: latexwc.c file_funcs.o walk_file.o bracket_stack.o tag_array.o
	gcc -g -O0 -o test/$@ $< build/file_funcs.o build/walk_file.o build/bracket_stack.o build/tag_array.o

latexwc: latexwc.c file_funcs.o walk_file.o bracket_stack.o tag_array.o
	gcc -o ~/bin/$@ $< build/file_funcs.o build/walk_file.o build/bracket_stack.o build/tag_array.o