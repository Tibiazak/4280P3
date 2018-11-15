//
// Created by fof_z on 11/14/2018.
//

#include <stdlib.h>
#include <stdio.h>
#include "token.h"
#include "scanner.h"

void expr();
void stat();
void vars();

token tk;
FILE * fp;

void RO()
{
    if(tk.tokenID == lessTk)
    {
        tk = scan(fp);
        if(tk.tokenID == equalTk)
        {
            tk = scan(fp);
            return;
        }
        else
        {
            return;
        }
    }
    else if(tk.tokenID == greaterTk)
    {
        tk = scan(fp);
        if(tk.tokenID == equalTk)
        {
            tk = scan(fp);
            return;
        }
        else
        {
            return;
        }
    }
    else if(tk.tokenID == equalTk)
    {
        tk = scan(fp);
        if(tk.tokenID == equalTk)
        {
            tk = scan(fp);
            return;
        }
        else
        {
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

void assign()
{
    if(tk.tokenID == identifierTk)
    {
        tk = scan(fp);
        if(tk.tokenID == equalTk)
        {
            tk = scan(fp);
            expr();
            if(tk.tokenID == colonTk)
            {
                tk = scan(fp);
                return;
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

void loop()
{
    if(tk.tokenID == iterTk)
    {
        tk = scan(fp);
        if(tk.tokenID == openparenTk)
        {
            tk = scan(fp);
            expr();
            RO();
            expr();
            if(tk.tokenID == closeparenTk)
            {
                tk = scan(fp);
                stat();
                return;
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

void ifprod()
{
    if(tk.tokenID == condTk)
    {
        tk = scan(fp);
        if(tk.tokenID == openparenTk)
        {
            tk = scan(fp);
            expr();
            RO();
            expr();
            if(tk.tokenID == closeparenTk)
            {
                tk = scan(fp);
                stat();
                return;
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

void out()
{
    if(tk.tokenID == printTk)
    {
        tk = scan(fp);
        if(tk.tokenID == openparenTk)
        {
            tk = scan(fp);
            expr();
            if(tk.tokenID == closeparenTk)
            {
                tk = scan(fp);
                if(tk.tokenID == colonTk)
                {
                    tk = scan(fp);
                    return;
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

void in()
{
    if(tk.tokenID == readTk)
    {
        tk = scan(fp);
        if(tk.tokenID == openparenTk)
        {
            tk = scan(fp);
            if(tk.tokenID == identifierTk)
            {
                tk = scan(fp);
                if(tk.tokenID == closeparenTk)
                {
                    tk = scan(fp);
                    if(tk.tokenID == colonTk)
                    {
                        tk = scan(fp);
                        return;
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

void stat()
{
    if(tk.tokenID == readTk)
    {
        // in production
        in();
        return;
    }
    else if(tk.tokenID == printTk)
    {
        // out production
        out();
        return;
    }
    else if(tk.tokenID == condTk)
    {
        //if production
        ifprod();
        return;
    }
    else if(tk.tokenID == iterTk)
    {
        //loop production
        loop();
        return;
    }
    else if(tk.tokenID == identifierTk)
    {
        //assign production
        assign();
        return;
    }
    else
    {
        //error
        printf("Error! Expected read/print/cond/iter/identifier, received %s\n", tokenArr[tk.tokenID]);
        fclose(fp);
        exit(1);
    }
}

void mstat()
{
    if(tk.tokenID == endTk)
    {
        //empty production, return
        return;
    }
    else
    {
        stat();
        mstat();
        return;
    }
}

void stats()
{
    stat();
    mstat();
    return;
}

void block()
{
    if(tk.tokenID == beginTk)
    {
        tk = scan(fp);
        vars();
        stats();
        if(tk.tokenID == endTk)
        {
            tk = scan(fp);
            return;
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

void R()
{
    if(tk.tokenID == openparenTk)
    {
        tk = scan(fp);
        expr();
        if(tk.tokenID == closeparenTk)
        {
            tk = scan(fp);
            return;
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
        tk = scan(fp);
        return;
    }
    else if(tk.tokenID == integerTk)
    {
        tk = scan(fp);
        return;
    }
    else
    {
        printf("Error! Expected integer or identifier or openparentk, received %s\n", tokenArr[tk.tokenID]);
        fclose(fp);
        exit(1);
    }
}

void M()
{
    if(tk.tokenID == minusTk)
    {
        tk = scan(fp);
        M();
        return;
    }
    else
    {
        R();
        return;
    }
}

void A()
{
    M();
    if(tk.tokenID == plusTk)
    {
        tk = scan(fp);
        A();
        return;
    }
    else if(tk.tokenID == minusTk)
    {
        tk = scan(fp);
        A();
        return;
    }
    else
    {
        return;
    }
}

void expr()
{
    A();
    if(tk.tokenID == slashTk)
    {
        tk = scan(fp);
        expr();
        return;
    }
    else if(tk.tokenID == starTk)
    {
        tk = scan(fp);
        expr();
        return;
    }
    else
    {
        return;
    }
}

void vars()
{
    if(tk.tokenID == letTk)
    {
        tk = scan(fp);
        if(tk.tokenID == identifierTk)
        {
            tk = scan(fp);
            if(tk.tokenID == equalTk)
            {
                tk = scan(fp);
                if(tk.tokenID == integerTk)
                {
                    tk = scan(fp);
                    vars();
                    return;
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
        return;
    }
}

void program()
{
    if(tk.tokenID == voidTk)
    {
        tk = scan(fp);
        vars();
        block();
        return;
    }
    else
    {
        printf("Error: expected \"void\", got %s\n", tokenArr[tk.tokenID]);
        fclose(fp);
        exit(1);
    }
}

void parser(FILE * fptr)
{
    fp = fptr;
    tk = scan(fp);
    program();
    if(tk.tokenID == eofTk)
    {
        printf("Parse good\n");
    }
    else
    {
        printf("ERROR, no EOF but end of parse\n");
        fclose(fp);
        exit(1);
    }
    return;
}