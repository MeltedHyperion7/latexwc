#include <stdlib.h>
#include <stdbool.h>

#include "bracket_stack.h"

#include "walk_file.h"

int getCount(char* contents, long length) {
    char c;
    int lastWordLength = 0, count = 0;
    for(int i = 0, end = length + 1; i < end; i++) {
        c = contents[i];
        switch(c) {
        case ' ':
        case '\n':
        case '\t': // should this be here?
        case '\0':
            if(lastWordLength > 0) {
                count++;
                lastWordLength = 0;
            }
            break;
        default:
            lastWordLength++;
            break;
        }
    }

    return count;
}