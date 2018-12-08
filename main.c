// Joshua Bearden
// CS4280 Project 3
// Main just determines where the input is coming from,
// validates the filename, opens the file,
// and then calls the parser, which calls the scanner.
// It then calls the function to traverse the parse tree
// and the static semantics function.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "parsenode.h"
#include <stdbool.h>
#include "codegen.h"

int main(int argc, char * argv[])
{
    FILE * fp;
    char file[30];
//    char * filename;

    // Check command line arguments
    if (argc == 1)
    {
        printf("No file given, read from stdin.\n");
        fp = stdin;
//        filename = "out";
    }
    else if (argc == 2) // If there is a file listed
    {
//        printf("File listed: %s.fs18\n", argv[argc-1]);
        strcpy(file, argv[1]);
        strcat(file, ".fs18");
        fp = fopen(file, "r");
//        filename = argv[1];
    }
    else // too many arguments
    {
        printf("ERROR: takes a single command line argument with a file name\n");
        exit(1);
    }

    // if the file was unable to be opened, error
    if (fp != NULL)
    {
        fseek(fp, 0, SEEK_END);
        int size = ftell(fp);
        if (size == 0)
        {
            printf("Error, no input!\n");
            exit(1);
        }
        fseek(fp, 0, SEEK_SET);
    }
    else
    {
        printf("Error, invalid file!\n");
        exit(1);
    }

//    token newtoken;
//    bool done = false;
//    while(!done)
//    {
//        newtoken = scan(fp);
//        if(newtoken.tokenID == eofTk)
//        {
//            printf("%s %s\n", tokenArr[newtoken.tokenID], newtoken.tokenInstance);
//            done = true;
//        }
//        else
//        {
//            printf("%s %s\n", tokenArr[newtoken.tokenID], newtoken.tokenInstance);
//        }
//    }

    parseNode * tree = parser(fp);
    inOrderTraversal(tree);
    staticSemantics(tree);
    genCode(tree);
    freeTree(tree);

    // Close the file unless the file is stdin
    if (fp != stdin)
    {
        fclose(fp);
    }
    return 0;
}
