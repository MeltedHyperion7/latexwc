#include <stdbool.h>

#include "tag_array.h"

#ifndef LATEXWC_H_
#define LATEXWC_H_

#define CONFIG_FILE_PATH "/home/MeltedHyperion7/.latexwc"
#define APPROX_TEXT_TAG_COUNT 10

bool getContents(const char* filename, char** contents, long* length);
tagArray* getTextTags();

#endif