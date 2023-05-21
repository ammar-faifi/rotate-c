#include "parser.h"
#include "errmsgs.h"
#include "type.h"

/*
 *
 * Internal private functions definitions
 *
 */
internal u8 parse_imports(Parser *);

/*
 *
 * Public functions
 *
 */
Parser
parser_init(Lexer *l)
{
    UNUSED(l);
    TODO("implement parser");
    return (Parser){0};
}

u8
parser_director(Parser *p)
{
    UNUSED(p);
    TODO("implement parser director");
    return FAILURE;
}

void
parser_deinit(Parser *p)
{
    UNUSED(p);
    TODO("implement deinitializer");
}
