#pragma once

#include "lexer.h"

typedef struct Parser
{
    Lexer *lexer;

} Parser;

Parser parser_init(Lexer *lexer);

