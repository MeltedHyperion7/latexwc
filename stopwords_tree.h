#ifndef STOPWORDS_TREE_H_
#define STOPWORDS_TREE_H_

#include <stdbool.h>

struct treeNode;

typedef struct treeNodeArray {
    struct treeNode** arr;
    int size;
    int length;
} treeNodeArray;

typedef struct treeNode {
    char letter;
    bool isWordEnd;
    treeNodeArray* children;
} treeNode;

treeNode* newTreeNode(int childrenArraySize, char letter, bool isWordEnd);
treeNode* treeNodeAddChild(treeNode* node, char childLetter, bool isWordEnd);
treeNode* childWithLetter(treeNode* node, char letter);
bool stopwordsTreeSearch(treeNode* stopwordsTree, char* word);
void stopwordsTreeAddWord(treeNode* root, char* word);
void freeTreeNode(treeNode* node);
treeNode* loadStopwordsList(char* stopwordsFilePath);

#endif