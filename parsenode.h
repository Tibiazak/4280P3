//
// Created by fof_z on 11/26/2018.
//

#ifndef PARSENODE_H
#define PARSENODE_H

typedef struct
{
    char * nonTerm;
    char ident[8];
    char op[3];
    char integr[8];
    parseNode * leftSub;
    parseNode * midSub;
    parseNode * rightSub;
} parseNode;

#endif //PARSENODE_H
