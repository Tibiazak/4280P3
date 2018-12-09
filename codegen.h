/*
 * Joshua Bearden
 * CS4280
 * P4 Code Generation
 *
 * Header for codegen.c, just makes setupGenCode() a publicly accessible function
 */

#ifndef CODEGEN_H
#define CODEGEN_H

#include "parsenode.h"
void setupGenCode(parseNode *, FILE *);
#endif //CODEGEN_H
