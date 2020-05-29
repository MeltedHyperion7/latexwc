#ifndef _CARGS_H
#define _CARGS_H

#include <stdbool.h>

#include "flagLinkedList.h"

// ? is it called verbose flag
void addFlag(char flagCharacter, char* verboseFlag, bool required, bool* set, char** argument);

bool parseArguments(int argc, char const *argv[]);

#endif