#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// dedugging
#include <stdio.h>

#include "bracket_stack.h"
#include "tag_array.h"

#include "walk_file.h"

int getCount(char* contents, long length, tagArray* textTagList) {
    char c;

    // implement bracket stack. the first item is for text outside any brackets
    BracketStackItem* bracketStackTop = newBracketStackItem(true, '\0');
    BracketStackItem* newBracket;

    // we keep track of the length of the last word to deal with
    // double spaces and intermixed tabs
    int lastWordLength = 0;
    int count = 0;

    char lastWord[50];

    bool isTag = false;
    bool lastTagContainsText = false;

    for(int i = 0, end = length + 1; i < end; i++) {
        c = contents[i];
        switch(c) {
        case '\\':
        
        case '{':
        case '[':
        case '}':
        case ']':
        
        case ' ':
        case '(':
        case ')':
        case ',':
        case '.':
        case ';':
        case '!':

        case '\n':
        case '\t':
        case '\0':
            lastWord[lastWordLength] = '\0';
            
            if(isTag) {
                if(tagArrayContains(textTagList, lastWord)) {
                    lastTagContainsText = true;
                } else {
                    lastTagContainsText = false;
                }
            } else if(bracketStackTop->containsText && lastWordLength > 0) {
                count++;
                printf("Found word: %s\n", lastWord);
            }

            lastWordLength = 0;
            isTag = false;

            if(c == '\\') {
                isTag = true;
            } else if(c == '[' || c == '{') {
                newBracket = newBracketStackItem(lastTagContainsText, c);
                bracketStackTop = push(newBracket, bracketStackTop);
            } else if(c == '}' || c == ']') {
                if((c == '}' && bracketStackTop->bracketType != '{') || (c == ']' && bracketStackTop->bracketType != '[')) {
                    // mismatching brackets, quit
                    
                    freeStack(bracketStackTop);
                    return -1;
                }
                bracketStackTop = pop(bracketStackTop);
            }
            break;
        default:
            if(isalpha(c)) {
                lastWord[lastWordLength] = c;
                lastWordLength++;
            }
            break;
        }
    }

    freeStack(bracketStackTop);
    return count;
}