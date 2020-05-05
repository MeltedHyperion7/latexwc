#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "file_funcs.h"
#include "walk_file.h"
#include "tag_array.h"

int main(int argc, char const *argv[]) {
    tagArray* textTagList = getTextTags();
    
    if(textTagList == NULL) {
        fprintf(stderr, "Could not open .latexwc\n");
        exit(1);
    }

    char* contents = NULL;
    long length;

    if(argc != 2) {
        fprintf(stderr, "Expected 2 arguments.\n");
        exit(1);
    }

    char* filename = argv[1];

    if(!isTEXFile(filename)) {
        fprintf(stderr, "Expected a .tex file.\n");
        exit(1);
    }

    if(getContents(filename, &contents, &length)) {
        int count = getCount(contents, length, textTagList);
        if(count == -1) {
            printf("An error occured while parsing the file.");
        } else {
            printf("Word Count: %d\n", count);
        }

        freeTagArrayDeallocateElements(textTagList);
        free(contents);
    } else {
        fprintf(stderr, "Couldn't open file.\n");
    }
    return 0;
}
