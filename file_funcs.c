#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "file_funcs.h"
#include "tag_array.h"


bool isTEXFile(const char* filepath) {
    char* extension;
    int pathLength = strlen(filepath);

    if(pathLength < 5) {
        // path not long enough to contain extension
        return false;
    }

    extension = filepath + (pathLength - 4);
    if(strcmp(extension, ".tex") == 0) {
        return true;
    }

    return false;
}

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

tagArray* getTextTags(bool excludeHeadings) {
    char* contents;
    long length;

    char* homeDir = getenv("HOME");
    if(!homeDir) {
        // could not ascertain user's home directory
        return NULL;
    }

    char* textTagsFilePath;
    asprintf(&textTagsFilePath, "%s/.latexwc/.texttags", homeDir);
    // complete the file path by adding on the name of the tag list file

    if(!getContents(textTagsFilePath, &contents, &length)) {
        printf("Could not open: %s\n.", textTagsFilePath);
        free(textTagsFilePath);
        return NULL;
    }
    
    if(!excludeHeadings) {
        char* headingsFileContents;
        long headingsFileLength;
        char* headingsFilePath;

        asprintf(&headingsFilePath, "%s/.latexwc/.headings", homeDir);

        if(!getContents(headingsFilePath, &headingsFileContents, &headingsFileLength)) {
            printf("Could not open: %s\n.", headingsFilePath);
            free(headingsFilePath);
            return NULL;
        }

        // ensure that [contents] has enough space to store the addition text from .headings
        contents = realloc(contents, (length + headingsFileLength + 2) * sizeof(char));

        // ? will [headingsFileContents] need to be freed

        // ? can this be done easier with asprintf (can the same pointer be passed as the target as well as a value?)
        strcat(contents, "\n");
        strcat(contents, headingsFileContents);

        // ? is this correct
        length += headingsFileLength + 1;

        // cleanup
        free(headingsFilePath);
    }

    char c;
    // ? fix the potential buffer overflow with [tag]
    char tag[20];
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

                    tagArrayAppendAllocateElement(textTagList, tag);
                }
                lastTagLength = 0;
                break;
            default:
                tag[lastTagLength] = c;
                lastTagLength++;
                break;
        }
    }

    // cleanup
    free(contents);
    free(textTagsFilePath);

    return textTagList;
}