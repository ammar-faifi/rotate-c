#pragma once

#include "token.h"

decl_arr(Token);

typedef struct
{
    // lexer state variables
    uint index, len, line, begin_tkn_line, file_length;
    file_t *file; // not owned by the lexer
    LexErr error;
    uint save_index, save_line;
    arr(Token) tokens;
} Lexer;

// Lexer API
Lexer lexer_init(file_t *);
void lexer_deinit(Lexer *);
arr(Token) lexer_get_tokens(Lexer *lexer);
u8 lexer_lex(Lexer *);
void lexer_save_log(Lexer *, FILE *);
// local Lexer methods
// Check lexer.c
