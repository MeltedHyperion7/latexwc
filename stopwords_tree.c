#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "file_funcs.h"

#include "stopwords_tree.h"


// a dynamic array implementation (similar to tagArray) to hold the list of
// children of a node in the stopwords tree

treeNodeArray* newtreeNodeArray(int size) {
    /* Initialize and return an empty tag array. */

    treeNodeArray* tnArr = malloc(sizeof(treeNodeArray));
    tnArr->size = size;
    tnArr->length = 0;
    tnArr->arr = calloc(size, sizeof(treeNode*));

    return tnArr;
}

void treeNodeArrayResize(treeNodeArray* tnArr, int newSize) {
    /* Resizes the internal array of the tag array, but only if it can still fit all elements. */

    if(newSize >= tnArr->length) {
        tnArr->arr = realloc(tnArr->arr, newSize * sizeof(treeNode*));
        tnArr->size = newSize;
    }
}

void treeNodeArrayAppend(treeNodeArray* tnArr, treeNode* node) {
    /* Add [node] to the end of [tnArr]. Resize if needed. */
    
    if(tnArr->length == tnArr->size) {
        // the +1 ensures that the array works even if it is initialized with 0 size
        tagArrayResize(tnArr, (tnArr->size * 2) + 1);
    }

    tnArr->arr[tnArr->length] = node;
    tnArr->length++;
}

treeNode* treeNodeArrayGet(treeNodeArray* tnArr, int n) {
    /* Returns the nth element of [tnArr]. Returns NULL if the index does not exist. */

    if(n >= tnArr->length || n < 0) {
        return NULL;
    }

    return tnArr->arr[n];
}

void treeNodeArraySet(treeNodeArray* tnArr, int n, treeNode* node) {
    if(n >= 0 && n < tnArr->length) {
        tnArr->arr[n] = node;
    }
}

bool treeNodeArrayContains(treeNodeArray* tnArr, treeNode* node) {
    for(int i = 0; i < tnArr->length; i++) {
        if(node->letter == (tnArr->arr[i])->letter) {
            return true;
        }
    }

    return false;
}

void freeTreeNodeArray(treeNodeArray* tnArr) {
    free(tnArr->arr);
    free(tnArr);
}

void freeTreeNodeArrayDeallocateElements(treeNodeArray* tnArr) {
    for(int i = 0; i < tnArr->length; i++) {
        free(tnArr->arr[i]);
    }

    free(tnArr->arr);
    free(tnArr);
}

// * treeNode implementation begins here
treeNode* newTreeNode(int childrenArraySize, char letter, bool isWordEnd) {
    /* Allocates a new treeNode and returns a pointer to it. */

    treeNode* node = malloc(sizeof(treeNode));
    node->children = newtreeNodeArray(childrenArraySize);
    node->letter = letter;
    node->isWordEnd = isWordEnd;

    return node;
}

treeNode* treeNodeAddChild(treeNode* node, char childLetter, bool isWordEnd) {
    /*
    Adds a new node with letter [letter] as a child of [node].
    Returns the newly created node.
    */

    // ? is initialization with 0 ok (probably)
    treeNode* newNode = newTreeNode(0, childLetter, isWordEnd);

    treeNodeArrayAppend(node->children, newNode);

    return newNode;
}

treeNode* childWithLetter(treeNode* node, char letter) {
    /*
    Returns the child of [node] that stores the letter [letter].
    Returns NULL if no such child exists.
    */

    for(int i = 0; i < node->children->length; i++) {
        if(treeNodeArrayGet(node->children, i)->letter == letter) {
            return treeNodeArrayGet(node->children, i);
        }
    }

    return NULL;
}

bool stopwordsTreeSearch(treeNode* stopwordsTree, char* word) {
    /* Returns true if [stopwordsTree] contains [word], else false. */

    treeNode* walk = stopwordsTree;

    // we search the list of children of the current node (in [walk]) to find
    // the next letter of the word until we find find the full word
    //  if at any point the next letter could not be found, the search returns false
    bool matchedLetter;
    char letterLowered;
    for(int wordIndex = 0, wordLength = strlen(word); wordIndex < wordLength; wordIndex++) {
        matchedLetter = false;
        letterLowered = tolower(word[wordIndex]);

        // ? can childWithLetter be used here
        for(int i = 0; i < walk->children->length; i++) {
            if(letterLowered == treeNodeArrayGet(walk->children, i)->letter) {
                walk = treeNodeArrayGet(walk->children, i);
                matchedLetter = true;
                break;
            }
        }

        if(!matchedLetter) {
            return false;
        }
    }

    // the node must be the end of a word that was added to the tree
    return walk->isWordEnd;
}

void stopwordsTreeAddWord(treeNode* root, char* word) {
    /* Adds [word] to the word tree rooted at [root]. */

    treeNode* currentNode = root;
    treeNode* nextNode;

    // go down the tree following the characters of [word]
    // if the next character does not have a node, create it
    for(int wordIndex = 0, wordLength = strlen(word); wordIndex < wordLength; wordIndex++) {
        nextNode = childWithLetter(currentNode, word[wordIndex]);
        if(nextNode == NULL) {
            // if the next letter of the word is not a child of the current node,
            // create it and make the new node the current node
            // if this is the last character word of the word, mark it as a word end
            currentNode = treeNodeAddChild(currentNode, word[wordIndex], (wordIndex == wordLength - 1 ? true : false));
        } else {
            currentNode = nextNode;
        }
    }
}

void freeTreeNode(treeNode* node) {
    /* Frees [node], including any children and their treeNodeArray lists. */

    if(node->children->length != 0) {
        // free all children
        for(int i = 0; i < node->children->length; i++) {
            freeTreeNode(treeNodeArrayGet(node->children, i));
        }
    }

    freeTreeNodeArray(node->children);
    free(node);
}

treeNode* loadStopwordsList(char* stopwordsFilePath) {
    /*
    Generates a tree from words from the file with path [stopwordsFilePath] and
    returns the root of the tree.
    */

    char* contents;
    long length;

    if(!getContents(stopwordsFilePath, &contents, &length)) {
        return NULL;
    }

    char c;
    int lastWordLength = 0;
    char lastWord[20];
    // ? could cause buffer overflow
    
    treeNode* root = newTreeNode(0, '\0', false);
    for(long i = 0, end = length + 1; i < end; i++) {
        c = contents[i];
        
        switch(c) {
            case ' ':
                // ignore
                break;
            case '\n':
            case '\0':
                // add to tree
                if(lastWordLength != 0) {
                    lastWord[lastWordLength] = '\0';

                    stopwordsTreeAddWord(root, lastWord);
                }
                lastWordLength = 0;
                break;
            default:
                lastWord[lastWordLength] = tolower(c);
                lastWordLength++;
                break;
        }
    }

    free(contents);

    return root;
}
