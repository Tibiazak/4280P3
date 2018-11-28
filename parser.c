//
// Joshua Bearden
// CS4280
// P2 Parser
// This file takes in the next token (from the scanner) and
// determines if the tokens are coming in a valid order.
// To-do: Generate parse tree
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "token.h"
#include "scanner.h"
#include "parsenode.h"

// Declaring some functions at the top
parseNode * expr();
parseNode * stat();
parseNode * vars();
parseNode * block();

token tk;
FILE * fp;
parseNode * treeBase;
char errorMsg[100];
bool error = false;


parseNode * init_node()
{
    parseNode * newNode = (parseNode *)malloc(sizeof(parseNode));
    newNode->leftSub = NULL;
    newNode->rightSub = NULL;
    newNode->midSub = NULL;
    return newNode;
}


void freeTree(parseNode * treePtr)
{
    if(treePtr->leftSub)
    {
        freeTree(treePtr->leftSub);
    }
    if(treePtr->rightSub)
    {
        freeTree(treePtr->rightSub);
    }
    if(treePtr->midSub)
    {
        freeTree(treePtr->midSub);
    }
    free(treePtr->leftSub);
    free(treePtr->rightSub);
    free(treePtr->midSub);
    return;
}


// Each of the following functions is named after a nonterminal from the BNF, it follows each production
// As written in the BNF.
void RO(parseNode * n)
{
    if(tk.tokenID == lessTk)
    {
        tk = scan(fp);
        if(tk.tokenID == equalTk)
        {
            strcpy(n->op, "< =");
            tk = scan(fp);
            return;
        }
        else
        {
            strcpy(n->op, "<");
            return;
        }
    }
    else if(tk.tokenID == greaterTk)
    {
        tk = scan(fp);
        if(tk.tokenID == equalTk)
        {
            strcpy(n->op, "> =");
            tk = scan(fp);
            return;
        }
        else
        {
            strcpy(n->op, ">");
            return;
        }
    }
    else if(tk.tokenID == equalTk)
    {
        tk = scan(fp);
        if(tk.tokenID == equalTk)
        {
            strcpy(n->op, "= =");
            tk = scan(fp);
            return;
        }
        else
        {
            strcpy(n->op, "=");
            return;
        }
    }
    else
    {
        strcpy(errorMsg, "Error! Expected <, or >, or =, received ");
        strcat(errorMsg, tokenArr[tk.tokenID]);
        error = true;
        return;
    }
}

parseNode * assign()
{
    parseNode * n;
    n = init_node();
    n->nonTerm = "assign";
    if(tk.tokenID == identifierTk)
    {
        strcpy(n->ident, tk.tokenInstance);
        tk = scan(fp);
        if(tk.tokenID == equalTk)
        {
            tk = scan(fp);
            n->midSub = expr();
            if(error)
            {
                return n;
            }

            if(tk.tokenID == colonTk)
            {
                tk = scan(fp);
                return n;
            }
            else
            {
                strcpy(errorMsg, "Error! Expected colontk, received ");
                strcat(errorMsg, tokenArr[tk.tokenID]);
                error = true;
                return n;
            }
        }
        else
        {
            strcpy(errorMsg, "Error! Expected equaltk, received ");
            strcat(errorMsg, tokenArr[tk.tokenID]);
            error = true;
            return n;
        }
    }
    else
    {
        strcpy(errorMsg, "Error! Expected identifier, received ");
        strcat(errorMsg, tokenArr[tk.tokenID]);
        error = true;
        return n;
    }
}

parseNode * loop()
{
    parseNode * n;
    n = init_node();
    n->nonTerm = "loop";
    if(tk.tokenID == iterTk)
    {
        tk = scan(fp);
        if(tk.tokenID == openparenTk)
        {
            tk = scan(fp);
            n->leftSub = expr();
            if(error)
            {
                return n;
            }

            RO(n);
            if(error)
            {
                return n;
            }

            n->midSub = expr();
            if(error)
            {
                return n;
            }

            if(tk.tokenID == closeparenTk)
            {
                tk = scan(fp);
                n->rightSub = stat();
                return n;
            }
            else
            {
                strcpy(errorMsg, "Error! Expected closeparentk, received ");
                strcat(errorMsg, tokenArr[tk.tokenID]);
                error = true;
                return n;
            }
        }
        else
        {
            strcpy(errorMsg, "Error! Expected openparentk, received ");
            strcat(errorMsg, tokenArr[tk.tokenID]);
            error = true;
            return n;
        }
    }
    else
    {
        strcpy(errorMsg, "Error! Expected itertk, received ");
        strcat(errorMsg, tokenArr[tk.tokenID]);
        error = true;
        return n;
    }
}

parseNode * ifprod()
{
    parseNode * n;
    n = init_node();
    n->nonTerm = "if";
    if(tk.tokenID == condTk)
    {
        tk = scan(fp);
        if(tk.tokenID == openparenTk)
        {
            tk = scan(fp);
            n->leftSub = expr();
            if(error)
            {
                return n;
            }

            RO(n);
            if(error)
            {
                return n;
            }

            n->midSub = expr();
            if(error)
            {
                return n;
            }

            if(tk.tokenID == closeparenTk)
            {
                tk = scan(fp);
                n->rightSub = stat();
                return n;
            }
            else
            {
                strcpy(errorMsg, "Error! Expected closeparentk, received ");
                strcat(errorMsg, tokenArr[tk.tokenID]);
                error = true;
                return n;
            }
        }
        else
        {
            strcpy(errorMsg, "Error! Expected openparentk, received ");
            strcat(errorMsg, tokenArr[tk.tokenID]);
            error = true;
            return n;
        }
    }
    else
    {
        strcpy(errorMsg, "Error! Expected condtk, received ");
        strcat(errorMsg, tokenArr[tk.tokenID]);
        error = true;
        return n;
    }
}

parseNode * out()
{
    parseNode * n;
    n = init_node();
    n->nonTerm = "out";
    if(tk.tokenID == printTk)
    {
        tk = scan(fp);
        if(tk.tokenID == openparenTk)
        {
            tk = scan(fp);
            n->midSub = expr();
            if(error)
            {
                return n;
            }
            if(tk.tokenID == closeparenTk)
            {
                tk = scan(fp);
                if(tk.tokenID == colonTk)
                {
                    tk = scan(fp);
                    return n;
                }
                else
                {
                    strcpy(errorMsg, "Error! Expected colontk, received ");
                    strcat(errorMsg, tokenArr[tk.tokenID]);
                    error = true;
                    return n;
                }
            }
            else
            {
                strcpy(errorMsg, "Error! Expected closeparentk, received ");
                strcat(errorMsg, tokenArr[tk.tokenID]);
                error = true;
                return n;
            }
        }
        else
        {
            strcpy(errorMsg, "Error! Expected openparentk, received ");
            strcat(errorMsg, tokenArr[tk.tokenID]);
            error = true;
            return n;
        }
    }
    else
    {
        strcpy(errorMsg, "Error! Expected printtk, received ");
        strcat(errorMsg, tokenArr[tk.tokenID]);
        error = true;
        return n;
    }
}

parseNode * in()
{
    parseNode * n;
    n = init_node();
    n->nonTerm = "in";
    if(tk.tokenID == readTk)
    {
        tk = scan(fp);
        if(tk.tokenID == openparenTk)
        {
            tk = scan(fp);
            if(tk.tokenID == identifierTk)
            {
                strcpy(n->ident, tk.tokenInstance);
                tk = scan(fp);
                if(tk.tokenID == closeparenTk)
                {
                    tk = scan(fp);
                    if(tk.tokenID == colonTk)
                    {
                        tk = scan(fp);
                        return n;
                    }
                    else
                    {
                        strcpy(errorMsg, "Error! Expected colontk, received ");
                        strcat(errorMsg, tokenArr[tk.tokenID]);
                        error = true;
                        return n;
                    }
                }
                else
                {
                    strcpy(errorMsg, "Error! Expected closeparentk, received ");
                    strcat(errorMsg, tokenArr[tk.tokenID]);
                    error = true;
                    return n;
                }
            }
            else
            {
                strcpy(errorMsg, "Error! Expected identifier, received ");
                strcat(errorMsg, tokenArr[tk.tokenID]);
                error = true;
                return n;
            }
        }
        else
        {
            strcpy(errorMsg, "Error! Expected openparentk, received ");
            strcat(errorMsg, tokenArr[tk.tokenID]);
            error = true;
            return n;
        }
    }
    else
    {
        strcpy(errorMsg, "Error! Expected readtk, received ");
        strcat(errorMsg, tokenArr[tk.tokenID]);
        error = true;
        return n;
    }
}

parseNode * stat()
{
    parseNode * n;
    n = init_node();
    n->nonTerm = "stat";
    if(tk.tokenID == readTk)
    {
        // in production
        n->midSub = in();
        return n;
    }
    else if(tk.tokenID == printTk)
    {
        // out production
        n->midSub = out();
        return n;
    }
    else if(tk.tokenID == condTk)
    {
        //if production
        n->midSub = ifprod();
        return n;
    }
    else if(tk.tokenID == iterTk)
    {
        //loop production
        n->midSub = loop();
        return n;
    }
    else if(tk.tokenID == identifierTk)
    {
        //assign production
        n->midSub = assign();
        return n;
    }
    else if(tk.tokenID == beginTk)
    {
        //block production
        n->midSub = block();
        return n;
    }
    else
    {
        //error
        strcpy(errorMsg, "Error! Expected read/print/cond/iter/identifier, received ");
        strcat(errorMsg, tokenArr[tk.tokenID]);
        error = true;
        return n;
    }
}

parseNode * mstat()
{
    parseNode * n;
    n = init_node();
    n->nonTerm = "mstat";
    if(tk.tokenID == endTk)
    {
        //empty production, return
        return n;
    }
    else
    {
        n->leftSub = stat();
        if(error)
        {
            return n;
        }
        n->rightSub = mstat();
        return n;
    }
}

parseNode * stats()
{
    parseNode * n;
    n = init_node();
    n->nonTerm = "stats";
    n->leftSub = stat();
    if(error)
    {
        return n;
    }
    n->rightSub = mstat();
    return n;
}

parseNode * block()
{
    parseNode * n;
    n = init_node();
    n->nonTerm = "block";
    if(tk.tokenID == beginTk)
    {
        tk = scan(fp);
        n->leftSub = vars();
        if(error)
        {
            return n;
        }
        n->rightSub = stats();
        if(error)
        {
            return n;
        }
        if(tk.tokenID == endTk)
        {
            tk = scan(fp);
            return n;
        }
        else
        {
            strcpy(errorMsg, "Error! Expected endtk, received ");
            strcat(errorMsg, tokenArr[tk.tokenID]);
            error = true;
            return n;
        }
    }
    else
    {
        strcpy(errorMsg, "Error! Expected begintk, received ");
        strcat(errorMsg, tokenArr[tk.tokenID]);
        error = true;
        return n;
    }
}

parseNode * R()
{
    parseNode * n;
    n = init_node();
    n->nonTerm = "R";
    if(tk.tokenID == openparenTk)
    {
        tk = scan(fp);
        n->midSub = expr();
        if(error)
        {
            return n;
        }
        if(tk.tokenID == closeparenTk)
        {
            tk = scan(fp);
            return n;
        }
        else
        {
            strcpy(errorMsg, "Error! Expected closeparentk, received ");
            strcat(errorMsg, tokenArr[tk.tokenID]);
            error = true;
            return n;
        }
    }
    else if(tk.tokenID == identifierTk)
    {
        strcpy(n->ident, tk.tokenInstance);
        tk = scan(fp);
        return n;
    }
    else if(tk.tokenID == integerTk)
    {
        strcpy(n->integr, tk.tokenInstance);
        tk = scan(fp);
        return n;
    }
    else
    {
        strcpy(errorMsg, "Error! Expected integer or identifier or openparentk, received ");
        strcat(errorMsg, tokenArr[tk.tokenID]);
        error = true;
        return n;
    }
}

parseNode * M()
{
    parseNode * n;
    n = init_node();
    n->nonTerm = "M";
    if(tk.tokenID == minusTk)
    {
        strcpy(n->op, "-");
        tk = scan(fp);
        n->midSub = M();
        return n;
    }
    else
    {
        n->midSub = R();
        return n;
    }
}

parseNode * A()
{
    parseNode * n;
    n = init_node();
    n->nonTerm = "A";
    n->leftSub = M();
    if(error)
    {
        return n;
    }
    if(tk.tokenID == plusTk)
    {
        strcpy(n->op, "+");
        tk = scan(fp);
        n->rightSub = A();
        return n;
    }
    else if(tk.tokenID == minusTk)
    {
        strcpy(n->op, "-");
        tk = scan(fp);
        n->rightSub = A();
        return n;
    }
    else
    {
        return n;
    }
}

parseNode * expr()
{
    parseNode * n;
    n = init_node();
    n->nonTerm = "expr";
    n->leftSub = A();
    if(error)
    {
        return n;
    }
    if(tk.tokenID == slashTk)
    {
        strcpy(n->op, "/");
        tk = scan(fp);
        n->rightSub = expr();
        return n;
    }
    else if(tk.tokenID == starTk)
    {
        strcpy(n->op, "*");
        tk = scan(fp);
        n->rightSub = expr();
        return n;
    }
    else
    {
        return n;
    }
}

parseNode * vars()
{
    parseNode * n;
    n = init_node();
    n->nonTerm = "vars";
    if(tk.tokenID == letTk)
    {
        tk = scan(fp);
        if(tk.tokenID == identifierTk)
        {
            strcpy(n->ident, tk.tokenInstance);
            tk = scan(fp);
            if(tk.tokenID == equalTk)
            {
                tk = scan(fp);
                if(tk.tokenID == integerTk)
                {
                    strcpy(n->integr, tk.tokenInstance);
                    tk = scan(fp);
                    n->midSub = vars();
                    return n;
                }
                else
                {
                    strcpy(errorMsg, "Error! Expected integer, received ");
                    strcat(errorMsg, tokenArr[tk.tokenID]);
                    error = true;
                    return n;
                }
            }
            else
            {
                strcpy(errorMsg, "Error! Expected equaltk, received ");
                strcat(errorMsg, tokenArr[tk.tokenID]);
                error = true;
                return n;
            }
        }
        else
        {
            strcpy(errorMsg, "Error! Expected identifier, received ");
            strcat(errorMsg, tokenArr[tk.tokenID]);
            error = true;
            return n;
        }
    }
    else
    {
        // empty production, return
        return n;
    }
}

parseNode * program()
{
    parseNode * n;
    n = init_node();
    n->nonTerm = "program";
    if(tk.tokenID == voidTk)
    {
        tk = scan(fp);
        n->leftSub = vars();
        n->rightSub = block();
        if(error)
        {
            freeTree(n);
            fclose(fp);
            printf(errorMsg);
            printf("\n");
            exit(1);
        }
        return n;
    }
    else
    {
        printf("Error: expected \"void\", got %s\n", tokenArr[tk.tokenID]);
        freeTree(n);
        fclose(fp);
        exit(1);
    }
}

parseNode * parser(FILE * fptr)
{
    fp = fptr;
    tk = scan(fp);
    parseNode * n;
    n = program();
    if(tk.tokenID == eofTk)
    {
        printf("Parse good\n");
    }
    else
    {
        printf("ERROR, program continues after final \"end\"\n");
        freeTree(n);
        fclose(fp);
        exit(1);
    }
    return n;
}