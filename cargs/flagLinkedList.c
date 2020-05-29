#include "flagLinkedList.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void addFlagLinkedNode(struct flagLinkedNode** node, struct flag flag) {
    /* Adds a new node at the end of the linked list. */

    struct flagLinkedNode* newNode = malloc(sizeof(struct flagLinkedNode));
    newNode->flag = flag;
    newNode->next = NULL;

    if(*node == NULL) {
        *node = newNode;
        return;
    }

    struct flagLinkedNode* walk = *node;
    struct flagLinkedNode* prev = NULL;

    while(walk != NULL) {
        prev = walk;
        walk = walk->next;
    }

    prev->next = newNode;
}

struct flagLinkedNode* findNodeFromFlagCharacter(struct flagLinkedNode* node, char flagCharacter) {
    /*
    Finds the node that stores the flag entry with the flag character [flagCharacter].
    Returns NULL if no such entry exists.
    */

    struct flagLinkedNode* walk = node;
    while(walk != NULL) {
        if(walk->flag.flagCharacter == flagCharacter) {
            return walk;
        }

        walk = walk->next;
    }

    return NULL;
}

struct flagLinkedNode* findNodeFromVerboseFlag(struct flagLinkedNode* node, char* verboseFlag) {
    /*
    Finds the node that stores the flag entry with the verbose flag [verboseFlag].
    Returns NULL if no such entry exists.
    */

    struct flagLinkedNode* walk = node;

    if(verboseFlag != NULL) {
        while(walk != NULL) {
            if(walk->flag.verboseFlag != NULL && strcmp(walk->flag.verboseFlag, verboseFlag) == 0) {
                return walk;
            }

            walk = walk->next;
        }
    }

    return NULL;
}

void freeFlagLinkedNode(struct flagLinkedNode* node) {
    /* Frees the linked list beginning at [node]. */

    struct flagLinkedNode* walk = node, *temp;

    while(walk != NULL) {
        temp = walk;
        walk = walk->next;
        free(temp);
    }
}

struct flagLinkedNode* findNextUnsetNonFlagArgument(struct flagLinkedNode* node) {
    /*
    Finds the first unset flag registered with the '.' character.
    These arguments are passed without an accompanying flag.
    Returns NULL if no such flag is registered.
    */

    struct flagLinkedNode* walk = node;

    while(walk != NULL) {
        if(walk->flag.flagCharacter == '.' && !(*(walk->flag.set))) {
            return walk;
        }
        walk = walk->next;
    }

    return NULL;
}
