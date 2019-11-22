#include <stdlib.h>
#include <stdbool.h>

#include "bracket_stack.h"

#include "walk_file.h"

int getCount(char* contents, long length) {
    char c;

    // we keep track of the length of the last word to deal with
    // double spaces and intermixed tabs
    int lastWordLength = 0;
    int count = 0;

    bool isTag = false;

    for(int i = 0, end = length + 1; i < end; i++) {
        c = contents[i];
        switch(c) {
        case '\\':
            isTag = true;
            break;
        case ' ':
        case '\n':
        case '\t':
        case '\0':

            if(!isTag && lastWordLength > 0) {
                count++;
            }

            lastWordLength = 0;
            isTag = false;
            break;
        default:
            lastWordLength++;
            break;
        }
    }

    return count;
}