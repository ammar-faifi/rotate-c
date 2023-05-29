#include "parser.h"
#include "errmsgs.h"
#include "type.h"

/*
 *
 * Internal private functions definitions
 *
 */
internal u8 parser_director(Parser *);
internal u8 parse_imports(Parser *);
internal u8 parse_functions(Parser *);
internal u8 parse_structs(Parser *);
internal u8 parse_enums(Parser *);
internal u8 parse_gl_vars(Parser *);
// useful parser utils
internal Token
current(Parser *p)
{
    return array_at(p->lexer->tokens, (p->index));
}

internal Token
next(Parser *p)
{
    return array_at(p->lexer->tokens, (p->index + 1));
}

internal Token
previous(Parser *p)
{
    return array_at(p->lexer->tokens, (p->index - 1));
}

/*
 *
 * Public functions
 *
 */
Parser
parser_init(Lexer *l)
{
    Parser parser = {0};
    parser.lexer  = l;
    parser.index  = 0;
    return parser;
}

u8
parser_parse(Parser *p)
{
    UNUSED(p);
    TODO("implement parser");
    return FAILURE;
}

void
parser_deinit(Parser *p)
{
    UNUSED(p);
    TODO("implement deinitializer");
}

/*
 *
 * internal functions
 *
 */
u8
parser_director(Parser *p)
{
    UNUSED(p);
    TODO("implement parser director");
    return FAILURE;
}

u8
parse_imports(Parser *p)
{
    UNUSED(p);
    TODO("implement parser imports");
    return FAILURE;
}

u8
parse_functions(Parser *p)
{
    UNUSED(p);
    TODO("implement parse functions");
    return FAILURE;
}

u8
parse_structs(Parser *p)
{
    UNUSED(p);
    TODO("implement parse structs");
    return FAILURE;
}

u8
parse_enums(Parser *p)
{
    UNUSED(p);
    TODO("implement parse enums");
    return FAILURE;
}

u8
parse_gl_vars(Parser *p)
{
    UNUSED(p);
    TODO("implement parse global variables");
    return FAILURE;
}
