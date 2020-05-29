#include "cargs.h"
#include "flagLinkedList.c"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct flagLinkedNode* cargs_flags_list = NULL;

void addFlag(char flagCharacter, char* verboseFlag, bool required, bool* set, char** argument) {
    struct flag flag = {
        flagCharacter,
        verboseFlag,
        required,
        set,
        argument
    };
    
    // initialize
    *(flag.set) = false;

    addFlagLinkedNode(&cargs_flags_list, flag);
}

bool parseArguments(int argc, char const *argv[]) {
    char* clArg;
    bool success = true;
    bool flagArgumentNext = false;

    // stores the ndoe of the current flag being read
    // if the flag required an argument, [flagNode] would retain its value
    // so that an argument can be matched to it on the next iteration
    struct flagLinkedNode* flagNode = NULL;

    for(int i = 1; i < argc; i++) {
        clArg = argv[i];
        if(flagArgumentNext) {
            // the last flag that was parsed takes an argument
            *(flagNode->flag.argument) = clArg;

            flagArgumentNext = false;
        } else {
            if(strlen(clArg) > 2 && clArg[0] == '-' && clArg[1] == '-') {
                // verbose flag
                char* verboseFlag = clArg + 2 * sizeof(char);
                flagNode = findNodeFromVerboseFlag(cargs_flags_list, verboseFlag);
                if(flagNode == NULL) {
                    success = false;
                } else {
                    *(flagNode->flag.set) = true;
                    flagArgumentNext = flagNode->flag.argument != NULL;
                }
            } else if(clArg[0] == '-') {
                // normal flag
                // ? what if something like -. is passed
                // ? that would clash with non flag arguments and mess things up

                if(strlen(clArg) != 2) {
                    success = false;
                }

                char flagCharacter = clArg[1];
                flagNode = findNodeFromFlagCharacter(cargs_flags_list, flagCharacter);
                if(flagNode == NULL) {
                    // unknown flag
                    success = false;
                } else {
                    *(flagNode->flag.set) = true;
                    flagArgumentNext = flagNode->flag.argument != NULL;
                }
            } else {
                // non flag argument

                flagNode = findNextUnsetNonFlagArgument(cargs_flags_list);
                if(flagNode == NULL) {
                    success = false;
                } else {
                    *(flagNode->flag.set) = true;
                    if(flagNode->flag.argument != NULL) {
                        *(flagNode->flag.argument) = clArg;
                    }
                }
            }
        }
    }

    if(flagArgumentNext) {
        // the last flag expected an argument but none was provided
        *(flagNode->flag.set) = false;
        success = false;
    }

    struct flagLinkedNode* walk = cargs_flags_list;

    while(walk != NULL) {
        if(walk->flag.required && !(*(walk->flag.set))) {
            // a required flag was not set
            success = false;
        }

        walk = walk->next;
    }

    freeFlagLinkedNode(cargs_flags_list);
    return success;

}
