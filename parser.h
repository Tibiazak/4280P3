//
// Joshua Bearden
// CS4280
// Header for Parser
//

#ifndef PARSER_H
#define PARSER_H

#include "parsenode.h"

void freeTree(parseNode *);
parseNode * parser(FILE *);

#endif //PARSER_H
