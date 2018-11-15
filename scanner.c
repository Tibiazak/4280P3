//
// Joshua Bearden
// CS4280
// P2 Scanner
// This file scans a file returning tokens based on the file read
// Line numbers are currently not counted (to-do).
//

#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Array of tokens in the same order as the enumerated type for simple output
char * tokenArr[] = {"begin", "end", "iter", "void", "var", "return", "read", "print",
                     "program", "cond", "then", "let", "=", ">", "<", ":", "+", "-", "*",
                     "/", "%", ".", "(", ")", ",", "{", "}", ";", "[", "]", "identifier", "integer", "EOF"};

// Main function
token scan(FILE * fp)
{
    char tok[8];
    token newtoken;
    int i;

    // get the next whitespace separated token from the file, finish if it is EOF
    if(fscanf(fp, "%s", tok) == EOF)
    {
        newtoken.tokenID = eofTk;
        strcpy(newtoken.tokenInstance, tokenArr[newtoken.tokenID]);
        return newtoken;
    }

    newtoken.line = 0;
    printf("Token is %s\n", tok);
    // Check what token it is, update the token info accordingly and return
    for (i = 0; i < 30; i++)
    {
        newtoken.tokenID = i;
//        printf("Comparing current token %s to ID %s\n", tok, tokenArr[i]);
        if(strcmp(tokenArr[i], tok) == 0)
        {
//            printf("Tokens are equal\n");
            strcpy(newtoken.tokenInstance, tok);
            return newtoken;
        }
    }

    // Checks for integer and identifier tokens
    if(isdigit(tok[0]))
    {
        for(i = 0; i < 8; i++)
        {
            if(isalpha(tok[i]) && (i < strlen(tok)))
            {
                printf("Error, invalid integer!\n");
                fclose(fp);
                exit(1);
            }
        }
        newtoken.tokenID = integerTk;
        strcpy(newtoken.tokenInstance, tok);
        return newtoken;
    }
    if(isalpha(tok[0]))
    {
        if(islower(tok[0]))
        {
            newtoken.tokenID = identifierTk;
            strcpy(newtoken.tokenInstance, tok);
            return newtoken;
        }
        printf("Error, invalid identifier\n");
        fclose(fp);
        exit(1);
    }
    return newtoken;
}