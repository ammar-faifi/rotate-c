#pragma once

#include "lexer.h"

typedef struct Parser
{
    Lexer *lexer;

} Parser;

Parser parser_init(Lexer *);
void parser_deinit(Parser *);
u8 parser_director(Parser *);
// internal methods are in parser.c
