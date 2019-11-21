#include <stdbool.h>

#ifndef BRACKET_STACK_H
#define BRACKET_STACK_H

typedef struct BracketStackItem {
    bool containsText;
    char bracketType;
    struct BracketStackItem* next;
} BracketStackItem;

BracketStackItem* newBracketStackItem(bool containsText, char bracketType);
BracketStackItem* push(BracketStackItem* newItem, BracketStackItem* top);
BracketStackItem* pop(BracketStackItem* top);
void freeStack(BracketStackItem* top);

#endif