#pragma once

#include "../include/arraylist.h"
#include "../include/common.h"
#include "../include/file.h"

typedef uint TknIdx;

typedef enum
{
    Tkn_Identifier = 0,   // ids
    Tkn_BuiltinFunc,      // @ids
    Tkn_To,               // ..
    Tkn_In,               // in
    Tkn_As,               // 'as'
    Tkn_Delete,           // 'delete'
    Tkn_Equal,            // =
    Tkn_Integer,          // refers to 10 digits ints
    Tkn_IntKeyword,       // 'int'
    Tkn_UintKeyword,      // 'uint'
    Tkn_Float,            // refer to floats
    Tkn_FloatKeyword,     // 'float'
    Tkn_String,           // refer to strings
    Tkn_Char,             // refers to chars
    Tkn_CharKeyword,      // 'char'
    Tkn_True,             // 'true'
    Tkn_False,            // 'false'
    Tkn_BoolKeyword,      // 'bool'
    Tkn_Colon,            // :
    Tkn_Function,         // 'fn'
    Tkn_PLUS,             // +
    Tkn_MINUS,            // -
    Tkn_Star,             // *
    Tkn_DIV,              // /
    Tkn_OpenParen,        // (
    Tkn_CloseParen,       // )
    Tkn_OpenCurly,        // {
    Tkn_CloseCurly,       // }
    Tkn_OpenSQRBrackets,  // [
    Tkn_CloseSQRBrackets, // ]
    Tkn_Return,           // 'return'
    Tkn_Import,           // 'import'
    Tkn_If,               // 'if'
    Tkn_Else,             // 'else'
    Tkn_For,              // 'for'
    Tkn_While,            // 'while'
    Tkn_Greater,          // >
    Tkn_GreaterEql,       // >=
    Tkn_Less,             // <
    Tkn_LessEql,          // <=
    Tkn_Dot,              // .
    Tkn_Not,              // "!"
    Tkn_NotEqual,         // "!="
    Tkn_And,              // 'and'
    Tkn_Or,               // 'or'
    Tkn_Comma,            // ,
    Tkn_Public,           // 'pub'
    Tkn_Switch,           // 'switch'
    Tkn_Enum,             // 'enum'
    Tkn_EqualEqual,       // ==
    Tkn_Break,            // 'break'
    Tkn_AddEqual,         // +=
    Tkn_SubEqual,         // -=
    Tkn_MultEqual,        // *=
    Tkn_DivEqual,         // /=
    Tkn_Struct,           // 'struct'
    Tkn_Ref,              // 'ref' // TODO later
    Tkn_Nil,              // `nil` basically null
    Tkn_Terminator,       // newline or semicolon
    Tkn_EOT,              // EOT - END OF TOKENS  (count too)
} TknType;                // enum TknType

cstr tkn_type_describe(const TknType type);

typedef struct
{
    uint index, length, line;
    TknType type;
} Token;

typedef enum
{
    // Unknown token/error (default)
    LE_UNKNOWN,
    LE_OUT_OF_MEMORY,
    LE_LEXER_INVALID_CHAR,
    LE_TOO_LONG_IDENTIFIER, // an ID can have up to a specific length
    LE_TOO_LONG_NUMBER,
    LE_TOO_LONG_STRING,
    LE_TABS,                  // Tabs are not supported
    LE_NOT_CLOSED_CHAR,       // Single quote not closed
    LE_NOT_VALID_ESCAPE_CHAR, // Double quote not closed
    LE_NOT_CLOSED_STRING,
    LE_FILE_EMPTY,          // File empty error
    LE_END_OF_FILE,         // End of file error
    LE_BAD_TOKEN_AT_GLOBAL, // forbidden token in global scope
    LE_NOT_CLOSED_COMMENT,
    LE_UNSUPPORTED,
} LexErr; // enum LexErr

cstr get_keyword_or_type(const Token);
cstr lexer_err_advice(const LexErr);
cstr lexer_err_msg(const LexErr);
bool is_token_type_length_variable(TknType);
bool is_token_a_number(TknType);
void log_token(FILE *const, const Token, cstr);
