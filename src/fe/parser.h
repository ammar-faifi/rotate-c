#pragma once

#include "lexer.h"

typedef struct Parser
{
    Lexer *lexer;
    u32 index;
} Parser;

Parser parser_init(Lexer *);
u8 parser_parse(Parser *);
void parser_deinit(Parser *);
// internal methods are in parser.c
