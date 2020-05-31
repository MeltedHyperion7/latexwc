#include <stdbool.h>

#ifndef TAG_ARRAY_H_
#define TAG_ARRAY_H_

typedef struct tagArray {
    char** arr;
    int size;
    int length;
} tagArray;

tagArray* newTagArray(int size);
void tagArrayResize(tagArray* tarr, int newSize);
void tagArrayAppend(tagArray* tarr, char* tag);
void tagArrayAppendAllocateElement(tagArray* tarr, char* tag);
char* tagArrayGet(tagArray* tarr, int n);
void tagArraySet(tagArray* tarr, int n, char* tag);
bool tagArrayContains(tagArray* tarr, char* tag);
void freeTagArray(tagArray* tarr);
void freeTagArrayDeallocateElements(tagArray* tarr);

#endif