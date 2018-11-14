//
// Created by fof_z on 10/28/2018.
//

#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char * tokenArr[] = {"begin", "end", "iter", "void", "var", "return", "read", "print",
                     "program", "cond", "then", "let", "=", ">", "<", ":", "+", "-", "*",
                     "/", "%", ".", "(", ")", ",", "{", "}", ";", "[", "]", "identifier", "integer", "EOF"};

token scan(FILE * fp)
{
    char tok[8];
    token newtoken;
    int i;

    if(fscanf(fp, "%s", tok) == EOF)
    {
        newtoken.tokenID = eofTk;
        strcpy(newtoken.tokenInstance, tokenArr[newtoken.tokenID]);
        return newtoken;
    }

    newtoken.line = 0;
    printf("Token is %s\n", tok);
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

    if(isdigit(tok[0]))
    {
        for(i = 0; i < 8; i++)
        {
            if(isalpha(tok[i]))
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