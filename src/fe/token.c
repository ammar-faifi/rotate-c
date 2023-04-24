#include "token.h"

// TODO: convert tokens to cstring funcs

cstr
tkn_type_describe(const TknType type)
{
    switch (type)
    {
        case Tkn_As: return "as";
        case Tkn_In: return "in";
        case Tkn_To: return "to";
        case Tkn_Delete: return "delete";
        case Tkn_CharKeyword: return "char";
        case Tkn_Identifier: return "identifier";
        case Tkn_BuiltinFunc: return "builtin_func";
        case Tkn_Equal: return "assign'='";
        case Tkn_Integer: return "integer";
        case Tkn_SemiColon: return "semicolon';'";
        case Tkn_Colon: return "colon':'";
        case Tkn_String: return "cstr";
        case Tkn_Function: return "function'fn'";
        case Tkn_PLUS: return "plus'+'";
        case Tkn_MINUS: return "minus'-'";
        case Tkn_Star: return "star'*'";
        case Tkn_DIV: return "divide'/'";
        case Tkn_OpenParen: return "openparen'('";
        case Tkn_CloseParen: return "closeparen')'";
        case Tkn_OpenCurly: return "opencurlybrkt'{'";
        case Tkn_CloseCurly: return "closecurlybrkt'}'";
        case Tkn_OpenSQRBrackets: return "opensqrbrkt']'";
        case Tkn_CloseSQRBrackets: return "closesqrbrkt'['";
        case Tkn_Char: return "char";
        case Tkn_Return: return "return";
        case Tkn_Import: return "import";
        case Tkn_If: return "if";
        case Tkn_Else: return "else";
        case Tkn_For: return "for";
        case Tkn_While: return "while";
        case Tkn_Greater: return "greater'>'";
        case Tkn_Less: return "less'<'";
        case Tkn_True: return "bool:true";
        case Tkn_False: return "bool:false";
        case Tkn_Dot: return "dot'.'";
        case Tkn_Not: return "not'!'";
        case Tkn_And: return "and";
        case Tkn_Or: return "or";
        case Tkn_Float: return "float";
        case Tkn_Comma: return "comma','";
        case Tkn_Public: return "public";
        case Tkn_NotEqual: return "`!=` not eql"; return "char_word";
        case Tkn_FloatKeyword: return "float_word";
        case Tkn_IntKeyword: return "int_word";
        case Tkn_Switch: return "switch";
        case Tkn_BoolKeyword: return "bool_word";
        case Tkn_EqualEqual: return "equality";
        case Tkn_Break: return "break";
        case Tkn_AddEqual: return "add_equal";
        case Tkn_DivEqual: return "divide_equal";
        case Tkn_MultEqual: return "multiply_equal";
        case Tkn_SubEqual: return "subtract_equal";
        case Tkn_Struct: return "struct";
        case Tkn_Ref: return "ref";
        case Tkn_Enum: return "enum";
        case Tkn_Nil: return "nil (null)";
        case Tkn_UintKeyword: return "uint";
        case Tkn_GreaterEql: return ">=";
        case Tkn_LessEql: return "<=";
        case Tkn_Void: return "void";
        case Tkn_EOT: return "End OF Tokens";

        default: return "UNKNOWN";
    }
}

cstr
get_keyword_or_type(const Token tkn)
{

    switch (tkn.type)
    {
        case Tkn_As: return "as";
        case Tkn_In: return "in";
        case Tkn_To: return "..";
        case Tkn_Delete: return "delete";
        case Tkn_GreaterEql: return ">=";
        case Tkn_LessEql: return "<=";
        case Tkn_UintKeyword: return "uint";
        case Tkn_Function: return "fn";
        case Tkn_If: return "if";
        case Tkn_Or: return "or";
        case Tkn_For: return "for";
        case Tkn_Public: return "pub";
        case Tkn_IntKeyword: return "int";
        case Tkn_Ref: return "ref";
        case Tkn_And: return "and";
        case Tkn_Nil: return "nil";
        case Tkn_Enum: return "enum";
        case Tkn_Else: return "else";
        case Tkn_True: return "true";
        case Tkn_CharKeyword: return "char";
        case Tkn_BoolKeyword: return "bool";
        case Tkn_While: return "while";
        case Tkn_False: return "false";
        case Tkn_Switch: return "switch";
        case Tkn_Break: return "break";
        case Tkn_Return: return "return";
        case Tkn_Import: return "import";
        case Tkn_FloatKeyword: return "float";
        case Tkn_Struct: return "struct";
        case Tkn_EqualEqual: return "==";
        case Tkn_Equal: return "=";
        case Tkn_Colon: return ":";
        case Tkn_SemiColon: return ";";
        case Tkn_AddEqual: return "+=";
        case Tkn_SubEqual: return "-=";
        case Tkn_MultEqual: return "*=";
        case Tkn_DivEqual: return "/=";
        case Tkn_PLUS: return "+";
        case Tkn_MINUS: return "-";
        case Tkn_Star: return "*";
        case Tkn_DIV: return "/";
        case Tkn_OpenParen: return "(";
        case Tkn_CloseParen: return ")";
        case Tkn_OpenCurly: return "{";
        case Tkn_CloseCurly: return "}";
        case Tkn_OpenSQRBrackets: return "[";
        case Tkn_CloseSQRBrackets: return "]";
        case Tkn_Greater: return ">";
        case Tkn_Less: return "<";
        case Tkn_Dot: return ".";
        case Tkn_NotEqual: return "!=";
        case Tkn_Not: return "!";
        case Tkn_Comma: return ",";
        case Tkn_Void: return "void";
        case Tkn_EOT: return "end_of_tokens";
        // Needs special handling
        case Tkn_Integer:
        case Tkn_Float:
        case Tkn_Identifier:
        case Tkn_String:
        case Tkn_Char:
        case Tkn_BuiltinFunc:
            ASSERT(false, "handle edge case of allocating for custom cstr representation");
            /*default: {
                return "TODO: IMPLEMENT";
            }*/
    }
    ASSERT(false, "implement Type to cstr situation");
}

cstr
lexer_err_msg(const LexErr error)
{
    switch (error)
    {
        case LE_OUT_OF_MEMORY: return "Out of memory";
        case LE_LEXER_INVALID_CHAR: return "Invalid character";
        case LE_TOO_LONG_IDENTIFIER: return "Identifier is too long";
        case LE_TOO_LONG_NUMBER: return "Number is too long";
        case LE_TOO_LONG_STRING: return "String is too long";
        case LE_NOT_CLOSED_CHAR: return "The char is not closed.";
        case LE_NOT_CLOSED_STRING: return "The cstr is not closed.";
        case LE_END_OF_FILE: return "reached end of file.";
        case LE_FILE_EMPTY: return "The file is empty.";
        case LE_BAD_TOKEN_AT_GLOBAL: return "Found global token at its forbidden scope";
        case LE_TABS: return "Tabs '\\t' are unsupported";
        case LE_NOT_VALID_ESCAPE_CHAR: return "Invalid escaped char";
        case LE_NOT_CLOSED_COMMENT: return "Comment not closed";
        case LE_UNSUPPORTED: break;
        case LE_UNKNOWN: break;
    }
    TODO("error msg implementation.");
    return "TODO: error msg implementation.";
}

cstr
lexer_err_advice(const LexErr error)
{
    switch (error)
    {
        case LE_NOT_VALID_ESCAPE_CHAR: return "Change the letter after \\";
        case LE_NOT_CLOSED_COMMENT: return "Close the comment with delimiter";
        case LE_LEXER_INVALID_CHAR: return "remove this character";
        case LE_OUT_OF_MEMORY: return "The compiler needs more memory";
        case LE_TOO_LONG_IDENTIFIER: return "Identifier must not exceed 100 characters";
        case LE_TOO_LONG_NUMBER: return "Number must not exceed 100 digits";
        case LE_TOO_LONG_STRING: return "String must not exceed (uint_MAX / 2) characters";
        case LE_NOT_CLOSED_CHAR: return "Close the char with a quote";
        case LE_NOT_CLOSED_STRING: return "Close the cstr with a double quote";
        case LE_END_OF_FILE: return "Needs more code for compiling";
        case LE_FILE_EMPTY: return "Do not compile empty files";
        case LE_BAD_TOKEN_AT_GLOBAL: return "Do not put this token in global scope";
        case LE_TABS: return "Convert the tabs to spaces";
        case LE_UNSUPPORTED: break;
        case LE_UNKNOWN: break;
    }
    TODO("error msg implementation.");
    return "TODO: error msg implementation.";
}
