//
// Created by fof_z on 12/7/2018.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parsenode.h"

#define MAXVARNUM 20
#define MAXSTRINGLENGTH 8


static int LabelCntr = 0;
static int VarCntr = 0;
typedef enum {VAR, LABEL} nameType;
static char Name[20];
char varArr[MAXVARNUM][MAXSTRINGLENGTH];
int varVal[MAXVARNUM];
int varNum;
FILE * fp;


static char *newName(nameType what)
{ if (what==VAR)
        sprintf(Name,"V%d",VarCntr++);    /* generate a new label as V0, V1, etc */
    else
        sprintf(Name,"L%d",LabelCntr++);            /* new lables as L0, L1, etc */
    return(Name);
}

void printVars()
{
    int i;
    for (i = 0; i < varNum; i++)
    {
        fprintf(fp, "%s %i\n", varArr[i], varVal[i]);
    }
    return;
}

void genCode(parseNode * tree)
{
    char label1[20];
    char label2[20];
    char tempName[20];

    if(!strcmp(tree->nonTerm, "program"))
    {
        genCode(tree->leftSub); //vars
        genCode(tree->rightSub); // block
        fprintf(fp, "STOP\n");
        printVars();
        return;
    }
    else if(!strcmp(tree->nonTerm, "block"))
    {
        genCode(tree->leftSub); // vars
        genCode(tree->rightSub); // stats
        return;
    }
    else if(!strcmp(tree->nonTerm, "vars"))
    {
        if(!strcmp(tree->ident, ""))
        {
            return; // <vars> -> empty production
        }
        else
        {
            if(varNum > 20)
            {
                printf("ERROR, too many variables\n");
                exit(1);
            }
            else
            {
                strcpy(varArr[varNum], tree->ident);
                varVal[varNum] = atoi(tree->integr);
                varNum = varNum + 1;
            }
            genCode(tree->midSub); // <vars>
            return;
        }
    }
    else if(!strcmp(tree->nonTerm, "expr"))
    {
        if(tree->rightSub)
        {
            genCode(tree->rightSub); // RHS of expr (<expr>)
            strcpy(tempName, newName(VAR));
            if(varNum > MAXVARNUM)
            {
                printf("ERROR too many variables\n");
                exit(1);
            }
            strcpy(varArr[varNum], tempName);
            varVal[varNum] = 0;
            varNum = varNum + 1;
            fprintf(fp, "STORE %s\n", tempName);
            genCode(tree->leftSub); // LHS of expr (<A>)
            if (!strcmp(tree->op, "*")) {
                fprintf(fp, "MULT %s\n", tempName);
            } else {
                fprintf(fp, "DIV %s\n", tempName);
            }
            return;
        }
        else
        {
            genCode(tree->leftSub); // the <expr> -> <A> production
            return;
        }
    }
    else if(!strcmp(tree->nonTerm, "A"))
    {
        if(tree->rightSub)
        {
            genCode(tree->rightSub); // RHS of A (<A>)
            strcpy(tempName, newName(VAR));
            if(varNum > MAXVARNUM)
            {
                printf("ERROR too many variables\n");
                exit(1);
            }
            strcpy(varArr[varNum], tempName);
            varVal[varNum] = 0;
            varNum = varNum + 1;
            fprintf(fp, "STORE %s\n", tempName);
            genCode(tree->leftSub); // LHS of expr <M>
            if(!strcmp(tree->op, "+"))
            {
                fprintf(fp, "ADD %s\n", tempName);
            }
            else
            {
                fprintf(fp, "SUB %s\n", tempName);
            }
            return;
        }
        else
        {
            genCode(tree->leftSub); // <A> -> <M> production
            return;
        }
    }
    else if(!strcmp(tree->nonTerm, "M"))
    {
        if(!strcmp(tree->op, "-"))
        {
            genCode(tree->midSub);
            fprintf(fp, "MULT -1\n");
            return;
        }
        else
        {
            genCode(tree->midSub);
            return;
        }
    }
    else if(!strcmp(tree->nonTerm, "R"))
    {
        if(tree->midSub)
        {
            genCode(tree->midSub);
            return;
        }
        else
        {
            if(strcmp(tree->ident, "")) // if identifier
            {
                fprintf(fp, "LOAD %s\n", tree->ident);
                return;
            }
            else
            {
                fprintf(fp, "LOAD %s\n", tree->integr);
                return;
            }
        }
    }
    else if(!strcmp(tree->nonTerm, "stats"))
    {
        genCode(tree->leftSub); // stat
        genCode(tree->rightSub); // mstat
        return;
    }
    else if(!strcmp(tree->nonTerm, "mstat"))
    {
        if(tree->leftSub)
        {
            genCode(tree->leftSub); // stat
            genCode(tree->rightSub); // mstat
            return;
        }
        else
        {
            return; //empty
        }
    }
    else if(!strcmp(tree->nonTerm, "stat"))
    {
        genCode(tree->midSub);
        return;
    }
    else if(!strcmp(tree->nonTerm, "in"))
    {
        fprintf(fp, "READ %s\n", tree->ident);
        return;
    }
    else if(!strcmp(tree->nonTerm, "out"))
    {
        genCode(tree->midSub); // expr
        strcpy(tempName, newName(VAR));
        if(varNum > MAXVARNUM)
        {
            printf("ERROR too many variables\n");
            exit(1);
        }
        strcpy(varArr[varNum], tempName);
        varVal[varNum] = 0;
        varNum = varNum + 1;
        fprintf(fp, "STORE %s\n", tempName);
        fprintf(fp, "WRITE %s\n", tempName);
        return;
    }
    else if(!strcmp(tree->nonTerm, "if"))
    {
        genCode(tree->midSub); // second <expr> of (<expr> <RO> <expr>)
        strcpy(tempName, newName(VAR));
        if(varNum > MAXVARNUM)
        {
            printf("ERROR too many variables\n");
            exit(1);
        }
        strcpy(varArr[varNum], tempName);
        varVal[varNum] = 0;
        varNum = varNum + 1;
        fprintf(fp, "STORE %s\n", tempName);
        genCode(tree->leftSub); // first <expr> of (<expr> <RO> <expr>)
        fprintf(fp, "SUB %s\n", tempName);
        strcpy(label1, newName(LABEL));
        if(!strcmp(tree->op, ">"))
        {
            fprintf(fp, "BRZNEG %s\n", label1);
        }
        else if(!strcmp(tree->op, "<"))
        {
            fprintf(fp, "BRZPOS %s\n", label1);
        }
        else if(!strcmp(tree->op, "> ="))
        {
            fprintf(fp, "BRNEG %s\n", label1);
        }
        else if(!strcmp(tree->op, "< ="))
        {
            fprintf(fp, "BRPOS %s\n", label1);
        }
        else if(!strcmp(tree->op, "="))
        {
            fprintf(fp, "BRPOS %s\n", label1);
            fprintf(fp, "BRNEG %s\n", label1);
        }
        else if(!strcmp(tree->op, "= ="))
        {
            fprintf(fp, "BRZERO %s\n", label1);
        }
        genCode(tree->rightSub); // code to execute if (<expr> <RO> <expr>) is true
        fprintf(fp, "%s: NOOP\n", label1);
        return;
    }
    else if(!strcmp(tree->nonTerm, "loop"))
    {
        strcpy(label1, newName(LABEL));
        strcpy(tempName, newName(VAR));
        if(varNum > MAXVARNUM)
        {
           printf("ERROR too many variables\n");
           exit(1);
        }
        strcpy(varArr[varNum], tempName);
        varVal[varNum] = 0;
        varNum = varNum + 1;
        strcpy(label2, newName(LABEL));
        fprintf(fp, "%s: NOOP\n", label1);
        genCode(tree->midSub);
        fprintf(fp, "STORE %s\n", tempName);
        genCode(tree->leftSub);
        fprintf(fp, "SUB %s\n", tempName);
        if(!strcmp(tree->op, ">"))
        {
            fprintf(fp, "BRZNEG %s\n", label2);
        }
        else if(!strcmp(tree->op, "<"))
        {
            fprintf(fp, "BRZPOS %s\n", label2);
        }
        else if(!strcmp(tree->op, "> ="))
        {
            fprintf(fp, "BRNEG %s\n", label2);
        }
        else if(!strcmp(tree->op, "< ="))
        {
            fprintf(fp, "BRPOS %s\n", label2);
        }
        else if(!strcmp(tree->op, "=")) // if equal
        {
            fprintf(fp, "BRPOS %s\n", label2);
            fprintf(fp, "BRNEG %s\n", label2);
        }
        else if(!strcmp(tree->op, "= =")) // if not equal
        {
            fprintf(fp, "BRZERO %s\n", label2);
        }
        genCode(tree->rightSub);
        fprintf(fp, "BR %s\n", label1);
        fprintf(fp, "%s: NOOP\n", label2);
        return;
    }
    else if(!strcmp(tree->nonTerm, "assign"))
    {
        genCode(tree->midSub); // expr
        fprintf(fp, "STORE %s\n", tree->ident);
        return;
    }
}

void setupGenCode(parseNode * tree, FILE * outfile)
{
    fp = outfile;
    genCode(tree);
    return;
}