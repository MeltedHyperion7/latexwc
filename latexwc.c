#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cargs/cargs.h"

#include "file_funcs.h"
#include "walk_file.h"
#include "tag_array.h"

int main(int argc, char const *argv[]) {
    // TODO run valgrind
    bool hFlagSet, fileProvided;
    char* filename;

    // if -h flag is set, headings and titles are excluded from the count
    addFlag('h', "exclude-headings", false, &hFlagSet, NULL);
    addFlag('.', NULL, true, &fileProvided, &filename);

    bool argParsingSuccess = parseArguments(argc, argv);

    if(!fileProvided) {
        fprintf(stderr, "No file path provided.\n");
        exit(1);
    }

    if(!argParsingSuccess) {
        fprintf(stderr, "Unknown argument.\n");
        exit(1);
    }

    // file provided wasn't a latex file
    if(!isTEXFile(filename)) {
        fprintf(stderr, "Expected a .tex file.\n");
        exit(1);
    }

    tagArray* textTagList = getTextTags(hFlagSet);
    
    if(textTagList == NULL) {
        fprintf(stderr, "Could not open .latexwc\n");
        exit(1);
    }

    char* contents = NULL;
    long length;

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
