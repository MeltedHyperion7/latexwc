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

    // printf("Tag List Length: %d\n", textTagList->length);
    // for(int i = 0, l = textTagList->length; i < l; i++) {
    //     printf("%s\n", textTagList->arr[i]);
    // }

    // printf("Searching for \"section\": %d\n", tagArrayContains(textTagList, "section"));
    // printf("Searching for \"subsection\": %d\n", tagArrayContains(textTagList, "subsection"));
    // printf("Searching for \"section1\": %d\n", tagArrayContains(textTagList, "section1"));
    // printf("Searching for \"secti o n\": %d\n", tagArrayContains(textTagList, "secti o n"));
    // printf("Searching for \"\": %d\n", tagArrayContains(textTagList, ""));
    // freeTagArrayDeallocateElements(textTagList);

    char* contents = NULL;
    long length;

    if(argc != 2) {
        fprintf(stderr, "Expected 2 arguments.\n");
        exit(1);
    }
    // bool success = getContents("./test/test1.txt", &contents, &length);
    if(getContents(argv[1], &contents, &length)) {
        printf("%s\n", contents);
        printf("Word Count: %d\n", getCount(contents, length, textTagList));

        freeTagArrayDeallocateElements(textTagList);
        free(contents);
    } else {
        fprintf(stderr, "Couldn't open file.\n");
    }
    return 0;
}
