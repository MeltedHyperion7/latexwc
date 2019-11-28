#include <stdlib.h>
#include <stdbool.h>

#include "bracket_stack.h"
#include "tag_array.h"

#ifndef WALK_FILE_H_
#define WALK_FILE_H_

int getCount(char* contents, long length, tagArray* textTagList);

#endif