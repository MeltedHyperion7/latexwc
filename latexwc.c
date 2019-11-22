#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "latexwc.h"

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