#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "file_funcs.h"
#include "tag_array.h"

// char* getFilePath(char* path) {
//     if(path[0] == '/') {

//     }
// }

bool getContents(const char* filename, char** contents, long* length) {
    FILE* file = fopen(filename, "r");
    if(!file) {
        return false;
    }

    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    rewind(file);

    *contents = malloc(*length + 1);
    fread(*contents, *length, 1, file);
    fclose(file);

    (*contents)[*length] = '\0';
    return true;
}

tagArray* getTextTags() {
    char* contents;
    long length;

    if(!getContents(CONFIG_FILE_PATH, &contents, &length)) {
        return NULL;
    }
    
    char c;
    char tag[20];
    // char* tagToAppend;
    int lastTagLength = 0;
    tagArray* textTagList = newTagArray(APPROX_TEXT_TAG_COUNT);

    for(long i = 0, end = length + 1; i < end; i++) {
        c = contents[i];
        
        switch(c) {
            case ' ':
                // ignore
                break;
            case '\n':
            case '\0':
                // add to array
                if(lastTagLength != 0) {
                    tag[lastTagLength] = '\0';

                    // more memory efficient way to do this?
                    // will this work?
                    // sprintf(tagToAppend, "%s", tag);
                    tagArrayAppendAllocateElement(textTagList, tag);
                }
                lastTagLength = 0;
                break;
            default:
                tag[lastTagLength] = c;
                lastTagLength += 1;
                break;
        }
    }

    free(contents);
    return textTagList;
    // add last word, if the file wasn't empty
}