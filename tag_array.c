#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "tag_array.h"

tagArray* newTagArray(int size) {
    /* Initialize and return an empty tag array. */

    tagArray* tarr = malloc(sizeof(tagArray));
    tarr->size = size;
    tarr->length = 0;
    tarr->arr = calloc(size, sizeof(char*));

    return tarr;
}

void tagArrayResize(tagArray* tarr, int newSize) {
    /* Resizes the internal array of the tag array, but only if it can still fit all elements. */

    if(newSize >= tarr->length) {
        tarr->arr = realloc(tarr->arr, newSize * sizeof(char*));
        tarr->size = newSize;
    }
}

void tagArrayAppend(tagArray* tarr, char* tag) {
    /* Add [tag] to the end of [tarr]. Resize if needed. */
    
    if(tarr->length == tarr->size) {
        tagArrayResize(tarr, tarr->size * 2);
    }

    tarr->arr[tarr->length] = tag;
    tarr->length++;
}

void tagArrayAppendAllocateElement(tagArray* tarr, char* tag) {
    /* Add [tag] to the end of [tarr]. Resize if needed. */
    
    if(tarr->length == tarr->size) {
        tagArrayResize(tarr, tarr->size * 2);
    }

    int tagLength = strlen(tag);
    char* newTag = malloc(sizeof(char) * (tagLength+1));
    strcpy(newTag, tag);
    // TODO need to add '\0'?

    tarr->arr[tarr->length] = newTag;
    tarr->length++;
}

char* tagArrayGet(tagArray* tarr, int n) {
    if(n >= tarr->length) {
        return NULL;
    }

    return tarr->arr[n];
}

void tagArraySet(tagArray* tarr, int n, char* tag) {
    if(n >= 0 && n < tarr->length) {
        tarr->arr[n] = tag;
    }
}

bool tagArrayContains(tagArray* tarr, char* tag) {
    for(int i = 0; i < tarr->length; i++) {
        if(strcmp(tag, tarr->arr[i]) == 0) {
            return true;
        }
    }

    return false;
}

void freeTagArray(tagArray* tarr) {
    // for(int i = 0; i < tarr->size; i++) {
    //     free(tarr->arr[i]);
    // }
    free(tarr->arr);
    free(tarr);
}

void freeTagArrayDeallocateElements(tagArray* tarr) {
    for(int i = 0; i < tarr->length; i++) {
        free(tarr->arr[i]);
    }

    free(tarr->arr);
    free(tarr);
}