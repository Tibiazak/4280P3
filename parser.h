//
// Joshua Bearden
// CS4280
// Header for Parser
//

#ifndef PARSER_H
#define PARSER_H

#include "parsenode.h"
#include <stdbool.h>

void freeTree(parseNode *);
parseNode * parser(FILE *);
void inOrderTraversal(parseNode *);
void staticSemantics(parseNode * , int );

#endif //PARSER_H
