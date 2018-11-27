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
        printf("Error! Expected <, or >, or =, received %s\n", tokenArr[tk.tokenID]);
        fclose(fp);
        exit(1);
    }
}

parseNode * assign()
{
    parseNode * n;
    n->nonTerm = "assign";
    if(tk.tokenID == identifierTk)
    {
        strcpy(n->ident, tk.tokenInstance);
        tk = scan(fp);
        if(tk.tokenID == equalTk)
        {
            tk = scan(fp);
            n->midSub = expr();
            if(tk.tokenID == colonTk)
            {
                tk = scan(fp);
                return n;
            }
            else
            {
                printf("Error! Expected colon, received %s\n", tokenArr[tk.tokenID]);
                fclose(fp);
                exit(1);
            }
        }
        else
        {
            printf("Error! Expected equaltk, received %s\n", tokenArr[tk.tokenID]);
            fclose(fp);
            exit(1);
        }
    }
    else
    {
        printf("Error! Expected identifier, received %s\n", tokenArr[tk.tokenID]);
        fclose(fp);
        exit(1);
    }
}

parseNode * loop()
{
    parseNode * n;
    n->nonTerm = "loop";
    if(tk.tokenID == iterTk)
    {
        tk = scan(fp);
        if(tk.tokenID == openparenTk)
        {
            tk = scan(fp);
            n->leftSub = expr();
            RO(n);
            n->midSub = expr();
            if(tk.tokenID == closeparenTk)
            {
                tk = scan(fp);
                n->rightSub = stat();
                return n;
            }
            else
            {
                printf("Error! Expected closeparentk, received %s\n", tokenArr[tk.tokenID]);
                fclose(fp);
                exit(1);
            }
        }
        else
        {
            printf("Error! Expected openparentk, received %s\n", tokenArr[tk.tokenID]);
            fclose(fp);
            exit(1);
        }
    }
    else
    {
        printf("Error! Expected itertk, received %s\n", tokenArr[tk.tokenID]);
        fclose(fp);
        exit(1);
    }
}

parseNode * ifprod()
{
    parseNode * n;
    n->nonTerm = "if";
    if(tk.tokenID == condTk)
    {
        tk = scan(fp);
        if(tk.tokenID == openparenTk)
        {
            tk = scan(fp);
            n->leftSub = expr();
            RO(n);
            n->midSub = expr();
            if(tk.tokenID == closeparenTk)
            {
                tk = scan(fp);
                n->rightSub = stat();
                return n;
            }
            else
            {
                printf("Error! Expected closeparentk, received %s\n", tokenArr[tk.tokenID]);
                fclose(fp);
                exit(1);
            }
        }
        else
        {
            printf("Error! Expected openparentk, received %s\n", tokenArr[tk.tokenID]);
            fclose(fp);
            exit(1);
        }
    }
    else
    {
        printf("Error! Expected condtk, received %s\n", tokenArr[tk.tokenID]);
        fclose(fp);
        exit(1);
    }
}

parseNode * out()
{
    parseNode * n;
    n->nonTerm = "out";
    if(tk.tokenID == printTk)
    {
        tk = scan(fp);
        if(tk.tokenID == openparenTk)
        {
            tk = scan(fp);
            n->midSub = expr();
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
                    printf("Error! Expected colon, received %s\n", tokenArr[tk.tokenID]);
                    fclose(fp);
                    exit(1);
                }
            }
            else
            {
                printf("Error! Expected closeparentk, received %s\n", tokenArr[tk.tokenID]);
                fclose(fp);
                exit(1);
            }
        }
        else
        {
            printf("Error! Expected openparentk, received %s\n", tokenArr[tk.tokenID]);
            fclose(fp);
            exit(1);
        }
    }
    else
    {
        printf("Error! Expected printtk, received %s\n", tokenArr[tk.tokenID]);
        fclose(fp);
        exit(1);
    }
}

parseNode * in()
{
    parseNode * n;
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
                        printf("Error! Expected colon, received %s\n", tokenArr[tk.tokenID]);
                        fclose(fp);
                        exit(1);
                    }
                }
                else
                {
                    printf("Error! Expected closeparentk, received %s\n", tokenArr[tk.tokenID]);
                    fclose(fp);
                    exit(1);
                }
            }
            else
            {
                printf("Error! Expected identifier, received %s\n", tokenArr[tk.tokenID]);
                fclose(fp);
                exit(1);
            }
        }
        else
        {
            printf("Error! Expected openparentk, received %s\n", tokenArr[tk.tokenID]);
            fclose(fp);
            exit(1);
        }
    }
    else
    {
        printf("Error! Expected readtk, received %s\n", tokenArr[tk.tokenID]);
        fclose(fp);
        exit(1);
    }
}

parseNode * stat()
{
    parseNode * n;
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
        printf("Error! Expected read/print/cond/iter/identifier, received %s\n", tokenArr[tk.tokenID]);
        fclose(fp);
        exit(1);
    }
}

parseNode * mstat()
{
    parseNode * n;
    n->nonTerm = "mstat";
    if(tk.tokenID == endTk)
    {
        //empty production, return
        return n;
    }
    else
    {
        n->leftSub = stat();
        n->rightSub = mstat();
        return n;
    }
}

parseNode * stats()
{
    parseNode * n;
    n->nonTerm = "stats";
    n->leftSub = stat();
    n->rightSub = mstat();
    return n;
}

parseNode * block()
{
    parseNode * n;
    n->nonTerm = "block";
    if(tk.tokenID == beginTk)
    {
        tk = scan(fp);
        n->leftSub = vars();
        n->rightSub = stats();
        if(tk.tokenID == endTk)
        {
            tk = scan(fp);
            return n;
        }
        else
        {
            printf("Error! Expected endtk, received %s\n", tokenArr[tk.tokenID]);
            fclose(fp);
            exit(1);
        }
    }
    else
    {
        printf("Error! Expected begintk, received %s\n", tokenArr[tk.tokenID]);
        fclose(fp);
        exit(1);
    }
}

parseNode * R()
{
    parseNode * n;
    n->nonTerm = "R";
    if(tk.tokenID == openparenTk)
    {
        tk = scan(fp);
        n->midSub = expr();
        if(tk.tokenID == closeparenTk)
        {
            tk = scan(fp);
            return n;
        }
        else
        {
            printf("Error! Expected closeparentk, received %s\n", tokenArr[tk.tokenID]);
            fclose(fp);
            exit(1);
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
        printf("Error! Expected integer or identifier or openparentk, received %s\n", tokenArr[tk.tokenID]);
        fclose(fp);
        exit(1);
    }
}

parseNode * M()
{
    parseNode * n;
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
    n->nonTerm = "A";
    n->leftSub = M();
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
    n->nonTerm = "expr";
    n->leftSub = A();
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
                    printf("Error! Expected integer, received %s\n", tokenArr[tk.tokenID]);
                    fclose(fp);
                    exit(1);
                }
            }
            else
            {
                printf("Error! Expected equaltk, received %s\n", tokenArr[tk.tokenID]);
                fclose(fp);
                exit(1);
            }
        }
        else
        {
            printf("Error! Expected identifier, received %s\n", tokenArr[tk.tokenID]);
            fclose(fp);
            exit(1);
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
    n->nonTerm = "program";
    if(tk.tokenID == voidTk)
    {
        tk = scan(fp);
        n->leftSub = vars();
        n->rightSub = block();
        return n;
    }
    else
    {
        printf("Error: expected \"void\", got %s\n", tokenArr[tk.tokenID]);
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
        fclose(fp);
        exit(1);
    }
    return n;
}