#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "bracket_stack.h"
#include "tag_array.h"
#include "stopwords_tree.h"

#include "walk_file.h"

int getCount(char* contents, long length, tagArray* textTagList, treeNode* stopwordsTree) {
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
    bool isComment = false;
    bool lastTagContainsText = false;
    bool isEscapedCharacter = false;

    for(int i = 0, end = length + 1; i < end; i++) {
        c = contents[i];
        switch(c) {
        case '%':
        case '\\':
        
        case '{':
        case '[':
        case '}':
        case ']':
        
        case ' ':
        case '?':
        case '(':
        case ')':
        case ',':
        case '.':
        case ';':
        case '!':

        case '\n':
        case '\t':
        case '\0':
            if(isEscapedCharacter && (c == '#' || c == '$' || c == '%' || c == '&' || c == '_' || c == '{' || c == '}')) {
                // other escape characters are treated as standard tags
                // ? how to handle these characters
                // right now we just ignore them
                isEscapedCharacter = false;
                isTag = false;
            } else {
                isEscapedCharacter = false;
                lastWord[lastWordLength] = '\0';
                
                if(isTag) {
                    if(tagArrayContains(textTagList, lastWord)) {
                        lastTagContainsText = true;
                    } else {
                        lastTagContainsText = false;
                    }
                } else if(!isComment && bracketStackTop->containsText && lastWordLength > 0) {
                    // ignore stopwords, if the necessary flag was set
                    // (if the flag was set, [stopwordsTree] would be non NULL)
                    if(stopwordsTree == NULL || !stopwordsTreeSearch(stopwordsTree, lastWord)) {
                        count++;
                    }
                }

                lastWordLength = 0;
                isTag = false;

                if(c == '\\') {
                    isTag = true;
                    isEscapedCharacter = true;
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
                } else if (c == '%') {
                    isComment = true;
                } else if(c == '\n') {
                    isComment = false;
                }
            }

            break;
        default:
            isEscapedCharacter = false;

            if(isalpha(c) && !isComment) {
                lastWord[lastWordLength] = c;
                lastWordLength++;
            }
            break;
        }
    }

    freeStack(bracketStackTop);
    return count;
}
