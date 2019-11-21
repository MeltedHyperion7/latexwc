bracket_stack.o: bracket_stack.c bracket_stack.h
	gcc -c -o build/$@ $<

bracket_stack_test: bracket_stack_test.c bracket_stack.o
	gcc -o test/$@ $< build/bracket_stack.o