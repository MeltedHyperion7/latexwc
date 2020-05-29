#ifndef _FLAG_LINKED_LIST_H_
#define _FLAG_LINKED_LIST_H_

#include <stdbool.h>

struct flag {
    char flagCharacter;
    char* verboseFlag;
    bool required;
    bool* set;
    char** argument;
};

struct flagLinkedNode {
    struct flag flag;
    struct flagLinkedNode* next;
};

void addFlagLinkedNode(struct flagLinkedNode** node, struct flag flag);
void freeFlagLinkedNode(struct flagLinkedNode* node);

struct flagLinkedNode* findNodeFromFlagCharacter(struct flagLinkedNode* node, char flagCharacter);
struct flagLinkedNode* findNodeFromVerboseFlag(struct flagLinkedNode* node, char* verboseFlag);
struct flagLinkedNode* findNextUnsetNonFlagArgument(struct flagLinkedNode* node);

bool requiredFlagNotSet(struct flagLinkedNode* node);

#endif
