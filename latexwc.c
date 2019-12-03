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
    if(!isTEXFile(argv[1])) {
        fprintf(stderr, "Expected a .tex file.\n");
        exit(1);
    }

    if(getContents(argv[1], &contents, &length)) {
        printf("Word Count: %d\n", getCount(contents, length, textTagList));

        freeTagArrayDeallocateElements(textTagList);
        free(contents);
    } else {
        fprintf(stderr, "Couldn't open file.\n");
    }
    return 0;
}
