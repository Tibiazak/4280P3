//
// Created by fof_z on 12/7/2018.
//

#include "parsenode.h"

#define MAXVARNUM 20;
#define MAXSTRINGLENGTH 8


static int LabelCntr = 0;
static int VarCntr = 0;
typedef enum {VAR, LABEL} nameType;
static char Name[20];
char varArr[MAXVARNUM][MAXSTRINGLENGTH];
int varVal[MAXVARNUM];
int varNum;


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
        printf("%s %i\n", varArr[i], varVal[i]);
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
        printf("STOP\n");
        printVars();
        return;
    }
    else if(!strcmp(tree->nonTerm, "block""))
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
            printf("STORE %s\n", tempName);
            genCode(tree->leftSub); // LHS of expr (<A>)
            if (!strcmp(tree->op, "*")) {
                printf("MULT %s\n", tempName);
            } else {
                printf("DIV %s\n", tempName);
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
            printf("STORE %s\n", tempName);
            genCode(tree->leftSub); // LHS of expr <M>
            if(!strcmp(tree->op, "+"))
            {
                printf("ADD %s\n", tempName);
            }
            else
            {
                printf("SUB %s\n", tempName);
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
            printf("MULT -1\n");
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
                printf("LOAD %s\n", tree->ident);
                return;
            }
            else
            {
                printf("LOAD %s\n", tree->integr);
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
        printf("READ %s\n", tree->ident);
        return;
    }
    else if(!strcmp(tree->nonTerm, "out"))
    {
        genCode(tree->midSub); // expr
        strcpy(tempName, newName(VAR));
        printf("STORE %s\n", tempName);
        printf("WRITE %s\n", tempName);
        return;
    }
    else if(!strcmp(tree->nonTerm, "if"))
    {
        genCode(tree->midSub); // second <expr> of (<expr> <RO> <expr>)
        strcpy(tempName, newName(VAR));
        printf("STORE %s\n", tempName);
        genCode(tree->leftSub); // first <expr> of (<expr> <RO> <expr>)
        printf("SUB %s\n", tempName);
        strcpy(label1, newName(LABEL));
        if(!strcmp(tree->op, ">"))
        {
            printf("BRZNEG %s\n", label1);
        }
        else if(!strcmp(tree->op, "<"))
        {
            printf("BRZPOS %s\n", label1);
        }
        else if(!strcmp(tree->op, "> ="))
        {
            printf("BRNEG %s\n", label1);
        }
        else if(!strcmp(tree->op, "< ="))
        {
            printf("BRPOS %s\n", label1);
        }
        else
        {
            printf("BRPOS %s\n", label1);
            printf("BRNEG %s\n", label1);
        }
        genCode(tree->rightSub); // code to execute if (<expr> <RO> <expr>) is true
        printf("%s: NOOP\n", label1);
        return;
    }
    else if(!strcmp(tree->nonTerm, "loop"))
    {
        strcpy(label1, newName(LABEL));
        strcpy(tempName, newName(VAR));
        strcpy(label2, newName(LABEL));
        printf("%s: NOOP\n", label1);
        genCode(tree->midSub);
        printf("STORE %s\n", tempName);
        genCode(tree->leftSub);
        printf("SUB %s\n", tempName);
        if(!strcmp(tree->op, ">"))
        {
            printf("BRZNEG %s\n", label2);
        }
        else if(!strcmp(tree->op, "<"))
        {
            printf("BRZPOS %s\n", label2);
        }
        else if(!strcmp(tree->op, "> ="))
        {
            printf("BRNEG %s\n", label2);
        }
        else if(!strcmp(tree->op, "< ="))
        {
            printf("BRPOS %s\n", label2);
        }
        else
        {
            printf("BRPOS %s\n", label2);
            printf("BRNEG %s\n", label2);
        }
        genCode(tree->rightSub);
        printf("BR %s\n", label1);
        printf("%s: NOOP\n", label2);
        return;
    }
    else if(!strcmp(tree->nonTerm, "assign"))
    {
        genCode(tree->midSub); // expr
        printf("STORE %s\n", tree->ident);
        return;
    }
}