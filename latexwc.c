#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cargs/cargs.h"

#include "file_funcs.h"
#include "walk_file.h"
#include "tag_array.h"
#include "stopwords_tree.h"

int main(int argc, char const *argv[]) {
    // TODO run valgrind
    bool helpFlagSet, headingsFlagSet, ignoreStopwordsFlagSet, ignoreStopwordsLongFlagSet, fileProvided;
    char* filename;

    // if -h flag is set, headings and titles are excluded from the count
    addFlag('h', "help", false, &helpFlagSet, NULL);
    addFlag('H', "exclude-headings", false, &headingsFlagSet, NULL);
    addFlag('.', NULL, true, &fileProvided, &filename);
    addFlag('s', "ignore-stopwords", false, &ignoreStopwordsFlagSet, NULL);
    addFlag('S', "ignore-stopwords-long", false, &ignoreStopwordsLongFlagSet, NULL);
    // TODO add flag for user's stopwords file

    bool argParsingSuccess = parseArguments(argc, argv);

    if(helpFlagSet) {
        // show help text
        printf("Usage: latexwc [options] file\n");
        printf("Options:\n");
        printf("  -h: Display this information.\n");
        printf("  -H: Ignore words in headings and titles.\n");
        printf("  -s: Ignore stopwords.\n");
        printf("  -S: Ignore stopwords. Uses a more expansive list of stopwords than -s.\n");
    } else {
        // TODO perform file checks before loading stopwords
        treeNode* stopwordsTree;
        if(ignoreStopwordsFlagSet && ignoreStopwordsLongFlagSet) {
            // ensure that both of -s and -S aren't set simultaneously
            fprintf(stderr, "Only one of -s and -S can be set.\n");
            exit(1);
        }
        if(ignoreStopwordsFlagSet) {
            char* stopwordsFilePath;
            asprintf(&stopwordsFilePath, "%s/.latexwc/.stopwords", getenv("HOME"));
            stopwordsTree = loadStopwordsList(stopwordsFilePath);
            if(stopwordsTree == NULL) {
                fprintf(stderr, "Could not load stopwords file.\n");
                free(stopwordsFilePath);
                exit(1);
            }

            free(stopwordsFilePath);
        } else if(ignoreStopwordsLongFlagSet) {
            char* stopwordsFilePath;
            asprintf(&stopwordsFilePath, "%s/.latexwc/.stopwords-long", getenv("HOME"));
            stopwordsTree = loadStopwordsList(stopwordsFilePath);
            if(stopwordsTree == NULL) {
                fprintf(stderr, "Could not load stopwords file.\n");
                free(stopwordsFilePath);
                exit(1);
            }

            free(stopwordsFilePath);
        } else {
            stopwordsTree = NULL;
        }

        if(!fileProvided) {
            fprintf(stderr, "No file path provided.\n");
            exit(1);
        }

        if(!argParsingSuccess) {
            fprintf(stderr, "Unknown argument.\n");
            exit(1);
        }

        // file provided wasn't a latex file
        if(!isTEXFile(filename)) {
            fprintf(stderr, "Expected a .tex file.\n");
            exit(1);
        }

        tagArray* textTagList = getTextTags(headingsFlagSet);
        
        if(textTagList == NULL) {
            fprintf(stderr, "Could not open .latexwc\n");
            exit(1);
        }

        char* contents = NULL;
        long length;

        if(getContents(filename, &contents, &length)) {
            int count = getCount(contents, length, textTagList, stopwordsTree);
            if(count == -1) {
                printf("An error occured while parsing the file.");
            } else {
                printf("Word Count: %d\n", count);
            }

            if(ignoreStopwordsFlagSet) {
                freeTreeNode(stopwordsTree);
            }
            freeTagArrayDeallocateElements(textTagList);
            free(contents);
        } else {
            fprintf(stderr, "Couldn't open file.\n");
        }
    }

    return 0;
}
