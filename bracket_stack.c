#include <stdlib.h>
#include <stdbool.h>

#include "bracket_stack.h"

BracketStackItem* newBracketStackItem(bool containsText, char bracketType) {
    BracketStackItem* newItem = malloc(sizeof(BracketStackItem));
    newItem->containsText = containsText;
    newItem->bracketType = bracketType;
    newItem->next = NULL;
    return newItem;
}

BracketStackItem* push(BracketStackItem* newItem, BracketStackItem* top) {
    newItem->next = top;
    return newItem;
}

BracketStackItem* pop(BracketStackItem* top) {
    BracketStackItem* newTop = top->next;
    free(top);
    return newTop;
}

void freeStack(BracketStackItem* top) {
    BracketStackItem* newTop;
    if(top != NULL) {
        newTop = top->next;
        free(top);
        top = newTop;
    }
}