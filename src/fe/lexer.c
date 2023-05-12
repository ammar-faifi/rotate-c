#include "lexer.h"

// PRIVATE LEXER METHODS
u8 lex_director(Lexer *);
u8 lex_chars(Lexer *);
u8 lex_numbers(Lexer *);
u8 lex_strings(Lexer *);
u8 lex_multiline_strings(Lexer *);
u8 lex_binary_numbers(Lexer *);
u8 lex_hex_numbers(Lexer *);
u8 lex_symbols(Lexer *);
u8 lex_builtin_funcs(Lexer *);
u8 lex_identifiers(Lexer *);
void lex_save_state(Lexer *);
void lex_restore_state_for_err(Lexer *);
u8 lex_report_error(Lexer *lexer);
u8 lex_add_token(Lexer *, TknType);
void lex_advance(Lexer *);
void lex_advance_len_inc(Lexer *);
void lex_advance_len_times(Lexer *);
char lex_peek(Lexer *);
char lex_past(Lexer *);
char lex_current(Lexer *);
bool lex_is_not_eof(Lexer *);
void lex_skip_whitespace(Lexer *);
bool lex_keyword_match(Lexer *, cstr, uint);

// Lexer API
// file must not be null and lexer owns the file ptr
Lexer
lexer_init(file_t *file)
{
    ASSERT_NULL(file, "Lexer File passed is a null pointer");
    Lexer lexer       = {0};
    lexer.index       = 0;
    lexer.len         = 0;
    lexer.line        = 1;
    lexer.file_length = file ? file->length : 0;
    lexer.file        = file;
    lexer.error       = LE_UNKNOWN;
    lexer.tokens      = make_arr(Token, file->length / 4);

    ASSERT_NULL(lexer.tokens, "Lexer vec of tokens passed is a null pointer");
    return lexer;
}

void
lexer_deinit(Lexer *lexer)
{
    arr_free(lexer->tokens);
}

void
lexer_save_log(Lexer *lexer, FILE *output)
{
    arr_foreach(lexer->tokens, el)
    {
        log_token(output, *el, lexer->file->contents);
    }
}

u8
lexer_lex(Lexer *lexer)
{
    for (;;)
    {
        switch (lex_director(lexer))
        {
            case SUCCESS: break;
            case DONE: {
                lexer->len = 0;
                for (u8 i = 0; i < EXTRA_NULL_TERMINATORS; ++i)
                    lex_add_token(lexer, Tkn_EOT);
                return SUCCESS;
            }
            case FAILURE: return lex_report_error(lexer);
        }
    }
    return FAILURE;
}

arr(Token) lexer_get_tokens(Lexer *lexer)
{
    return lexer->tokens;
}

// PRIVATE internals

void
lex_skip_whitespace(Lexer *lexer)
{
    for (;;)
    {
        const char c = lex_current(lexer);
        if (c == ' ')
        {
            lexer->index += 1;
        }
        else if (c == '\n')
        {
            lexer->index += 1;
            lexer->line += 1;
        }
        else
        {
            break;
        }
    }
}

u8
lex_director(Lexer *lexer)
{
    lex_skip_whitespace(lexer);
    lexer->len = 0, lexer->begin_tkn_line = lexer->line;
    lex_save_state(lexer);
    const char c = lex_current(lexer);
    // ints and floats
    if (isdigit(c)) return lex_numbers(lexer);
    // chars, and strings
    // TODO: Multiline strings
    if (c == '"') return lex_strings(lexer);
    if (c == '\'') return lex_chars(lexer);

    // NOTE: Idenitifiers, keywords and builtin functions
    if (c == '_' || isalpha(c)) return lex_identifiers(lexer);
    // if (c == '@') return lex_builtin_funcs(lexer);
    //  Symbols
    return lex_symbols(lexer);
}

u8
lex_identifiers(Lexer *lexer)
{
    lex_advance_len_inc(lexer);
    while (isalnum(lex_current(lexer)) || lex_current(lexer) == '_')
    {
        lex_advance_len_inc(lexer);
    }
    lexer->index -= lexer->len;
    TknType _type = Tkn_Identifier;

    // TODO: optimize searching for matching keywords
    switch (lexer->len)
    {
        case 2: {
            switch (lex_current(lexer))
            {
                case 'a':
                    if (lex_keyword_match(lexer, "as", 2)) _type = Tkn_As;
                    break;
                case 'f':
                    if (lex_keyword_match(lexer, "fn", 2)) _type = Tkn_Function;
                    break;
                case 'i':
                    if (lex_keyword_match(lexer, "if", 2))
                        _type = Tkn_If;
                    else if (lex_keyword_match(lexer, "in", 2))
                        _type = Tkn_In;
                    break;
                case 'o':
                    if (lex_keyword_match(lexer, "or", 2)) _type = Tkn_Or;
                    break;
                default: break;
            }

            break;
        }
        case 3: {
            switch (lex_current(lexer))
            {
                case 'f':
                    if (lex_keyword_match(lexer, "for", 3)) _type = Tkn_For;
                    break;
                case 'p':
                    if (lex_keyword_match(lexer, "pub", 3)) _type = Tkn_Public;
                    break;
                case 'i':
                    if (lex_keyword_match(lexer, "int", 3)) _type = Tkn_IntKeyword;
                    break;
                case 'r':
                    if (lex_keyword_match(lexer, "ref", 3)) _type = Tkn_Ref;
                    break;
                case 'a':
                    if (lex_keyword_match(lexer, "and", 3)) _type = Tkn_And;
                    break;
                case 'n':
                    if (lex_keyword_match(lexer, "nil", 3)) _type = Tkn_Nil;
                    break;
            }
            break;
        }
        case 4: {
            switch (lex_current(lexer))
            {
                case 'e': {
                    if (lex_keyword_match(lexer, "else", 4))
                        _type = Tkn_Else;
                    else if (lex_keyword_match(lexer, "enum", 4))
                        _type = Tkn_Enum;
                    break;
                }
                case 't':
                    if (lex_keyword_match(lexer, "true", 4)) _type = Tkn_True;
                    break;
                case 'c':
                    if (lex_keyword_match(lexer, "char", 4)) _type = Tkn_CharKeyword;
                    break;
                case 'b':
                    if (lex_keyword_match(lexer, "bool", 4)) _type = Tkn_BoolKeyword;
                    break;
                case 'u':
                    if (lex_keyword_match(lexer, "uint", 4)) _type = Tkn_UintKeyword;
                    break;
                case 'v':
                    if (lex_keyword_match(lexer, "void", 4)) _type = Tkn_Void;
                    break;
            }
            break;
        }
        case 5: {
            switch (lex_current(lexer))
            {
                case 'w':
                    if (lex_keyword_match(lexer, "while", 5)) _type = Tkn_While;
                    break;
                case 'f': {
                    if (lex_keyword_match(lexer, "false", 5))
                        _type = Tkn_False;
                    else if (lex_keyword_match(lexer, "float", 5))
                        _type = Tkn_FloatKeyword;
                    break;
                }
                case 'b':
                    if (lex_keyword_match(lexer, "break", 5)) _type = Tkn_Break;
                    break;
            }
            break;
        }
        case 6: {
            switch (lex_current(lexer))
            {
                case 'r':
                    if (lex_keyword_match(lexer, "return", 6)) _type = Tkn_Return;
                    break;
                case 'i':
                    if (lex_keyword_match(lexer, "import", 6)) _type = Tkn_Import;
                    break;
                case 'd':
                    if (lex_keyword_match(lexer, "delete", 6)) _type = Tkn_Delete;
                    break;
                case 's': {
                    if (lex_keyword_match(lexer, "struct", 6))
                        _type = Tkn_Struct;
                    else if (lex_keyword_match(lexer, "switch", 6))
                        _type = Tkn_Switch;
                    break;
                }
            }
            break;
        }
        default: break;
    }

    if (lexer->len > 100)
    {
        // log_error("identifier length is more than 128 chars");
        lexer->error = LE_TOO_LONG_IDENTIFIER;
        lex_restore_state_for_err(lexer);
        return FAILURE;
    }

    return lex_add_token(lexer, _type);
}

u8
lex_numbers(Lexer *lexer)
{
    const char c = lex_current(lexer);
    const char p = lex_peek(lexer);

    // 0x or 0b switch state of lexing to specific radix
    // x -> hexadecimal(r16) | b -> binary(r2)
    if (c == '0')
    {
        switch (p)
        {
            case 'x': return lex_hex_numbers(lexer);
            case 'b': return lex_binary_numbers(lexer);
            default: break;
        }
    }

    bool reached_dot = false;
    while (isdigit(lex_current(lexer)) || lex_current(lexer) == '.')
    {
        lex_advance_len_inc(lexer);
        if (lex_current(lexer) == '.')
        {
            if (reached_dot) break;
            reached_dot = true;
        }
    }

    if (lexer->len > 100)
    {
        log_error("number digits length is above 100");
        lex_restore_state_for_err(lexer);
        return FAILURE;
    }
    lexer->index -= lexer->len;
    return lex_add_token(lexer, reached_dot ? Tkn_Integer : Tkn_Float);
}

u8
lex_hex_numbers(Lexer *lexer)
{
    // skip '0x'
    lex_advance_len_inc(lexer);
    lex_advance_len_inc(lexer);
    while (isxdigit(lex_current(lexer)))
    {
        lex_advance_len_inc(lexer);
    }

    if (lexer->len > 0x20)
    {
        log_error("hex number digits length is above 32");
        lex_restore_state_for_err(lexer);
        return FAILURE;
    }
    lexer->index -= lexer->len;
    return lex_add_token(lexer, Tkn_Integer);
}

u8
lex_binary_numbers(Lexer *lexer)
{
    // skip '0b'
    lex_advance_len_inc(lexer);
    lex_advance_len_inc(lexer);
    while (lex_current(lexer) == '0' || lex_current(lexer) == '1')
    {
        lex_advance_len_inc(lexer);
    }

    if (lexer->len > 0x80)
    {
        log_error("binary number digits length is above 128");
        lex_restore_state_for_err(lexer);
        return FAILURE;
    }
    lexer->index -= lexer->len;
    return lex_add_token(lexer, Tkn_Integer);
}

u8
lex_strings(Lexer *lexer)
{
    lex_advance_len_inc(lexer);
    for (;;)
    {
        if (lex_current(lexer) == '\0')
        {
            lex_restore_state_for_err(lexer);
            lexer->error = LE_NOT_CLOSED_STRING;
            return FAILURE;
        }

        if (lex_current(lexer) == '"')
        {
            if (lex_past(lexer) == '\\')
            {
                lex_advance_len_inc(lexer);
                continue;
            }
            else
                break;
        }
        lex_advance_len_inc(lexer);
    }
    lex_advance_len_inc(lexer);

    if (lexer->len > (RUINT_MAX / 2))
    {
        lex_restore_state_for_err(lexer);
        log_error("A cstr is not allowed to be longer than (Uuint_MAX / 2)");
        lexer->error = LE_TOO_LONG_STRING;
        return FAILURE;
    }
    lexer->index -= lexer->len;
    return lex_add_token(lexer, Tkn_String);
}

u8
lex_chars(Lexer *lexer)
{
    lex_advance_len_inc(lexer);
    if (lex_current(lexer) != '\\' && lex_peek(lexer) == '\'')
    {
        lex_advance_len_inc(lexer);
        lex_advance_len_inc(lexer);
        lexer->index -= lexer->len;
        return lex_add_token(lexer, Tkn_Char);
    }
    else if (lex_current(lexer) == '\\')
    {
        lex_advance_len_inc(lexer);
        switch (lex_current(lexer))
        {
            case 'n':
            case 't':
            case 'r':
            case 'b':
            case 'f':
            case '\\':
            case '\'': lex_advance_len_inc(lexer); break;
            default:
                lexer->error = LE_NOT_VALID_ESCAPE_CHAR;
                lex_restore_state_for_err(lexer);
                return FAILURE;
        }
        if (lex_current(lexer) == '\'')
        {
            lex_advance_len_inc(lexer);
            lexer->index -= lexer->len;
            return lex_add_token(lexer, Tkn_Char);
        }
        else
        {
            lexer->error = LE_LEXER_INVALID_CHAR;
            lex_restore_state_for_err(lexer);
            return FAILURE;
        }
    }

    return FAILURE;
}

u8
lex_symbols(Lexer *lexer)
{
    const char c = lex_current(lexer);
    const char p = lex_peek(lexer);
    lexer->len   = 1;
    switch (c)
    {
        case '{': return lex_add_token(lexer, Tkn_OpenCurly);
        case '}': return lex_add_token(lexer, Tkn_CloseCurly);
        case '(': return lex_add_token(lexer, Tkn_OpenParen);
        case ')': return lex_add_token(lexer, Tkn_CloseParen);
        case '[': return lex_add_token(lexer, Tkn_OpenSQRBrackets);
        case ']': return lex_add_token(lexer, Tkn_CloseSQRBrackets);
        case ';': return lex_add_token(lexer, Tkn_SemiColon);
        case ',': return lex_add_token(lexer, Tkn_Comma);
        // TODO(5717) bug below needs to check an eql during peeking

        // more than one length char
        case '.': {
            if (p == '.')
            {
                lexer->len += 1;
                return lex_add_token(lexer, Tkn_To);
            }
            return lex_add_token(lexer, Tkn_Dot);
        }
        case ':': {
            return lex_add_token(lexer, Tkn_Colon);
        }
        case '>': {
            if (p == '=')
            {
                lexer->len += 1;
                return lex_add_token(lexer, Tkn_GreaterEql);
            }
            return lex_add_token(lexer, Tkn_Greater);
        }
        case '<': {
            if (p == '=')
            {
                lexer->len += 1;
                return lex_add_token(lexer, Tkn_LessEql);
            }
            return lex_add_token(lexer, Tkn_Less);
        }
        case '=': {
            if (p == '=')
            {
                lexer->len += 1;
                return lex_add_token(lexer, Tkn_EqualEqual);
            }
            return lex_add_token(lexer, Tkn_Equal);
        }
        case '+': {
            if (p == '=')
            {
                lexer->len += 1;
                return lex_add_token(lexer, Tkn_AddEqual);
            }
            return lex_add_token(lexer, Tkn_PLUS);
        }
        case '-': {
            if (p == '=')
            {
                lexer->len += 1;
                return lex_add_token(lexer, Tkn_SubEqual);
            }
            return lex_add_token(lexer, Tkn_MINUS);
        }
        case '*': {
            if (p == '=')
            {
                lexer->len += 1;
                return lex_add_token(lexer, Tkn_MultEqual);
            }
            return lex_add_token(lexer, Tkn_Star);
        }
        case '/': {
            if (p == '=')
            {
                lexer->len += 1;
                return lex_add_token(lexer, Tkn_DivEqual);
            }
            else if (p == '/')
            {
                //
                while (lex_is_not_eof(lexer) && lex_current(lexer) != '\n')
                    lex_advance(lexer);
                return SUCCESS;
            }
            else if (p == '*')
            {
                lex_advance(lexer);
                lex_advance(lexer);
                // TODO: Allow nested comments
                bool end_comment = false;
                while (lex_is_not_eof(lexer) && !end_comment)
                {
                    if (lex_current(lexer) == '*' && lex_peek(lexer) == '/')
                    {
                        lex_advance(lexer);
                        end_comment = true;
                    }
                    lex_advance(lexer);
                }
                return SUCCESS;
            }
            return lex_add_token(lexer, Tkn_DIV);
        }
        case '!': {
            if (p == '=')
            {
                lexer->len += 1;
                return lex_add_token(lexer, Tkn_NotEqual);
            }
            return lex_add_token(lexer, Tkn_Not);
        }
        case '#': {
            // this is for comments
            while (lex_current(lexer) != '\n' && lex_is_not_eof(lexer))
                lex_advance(lexer);
            lex_advance(lexer);
            return SUCCESS;
        }
        default: {
            switch (c)
            {
                case '\0': return DONE;
                case '\t': {
                    lexer->error = LE_TABS;
                    break;
                }
                case '\r': {
                    lex_advance(lexer);
                    return SUCCESS;
                }
                default: {
                    lexer->error = LE_LEXER_INVALID_CHAR;
                }
            }
        }
    }
    return FAILURE;
}

u8
lex_builtin_funcs(Lexer *lexer)
{
    lex_advance(lexer); // skip '@'

    // NOTE(5717): ONLY ALPHABET CHARS ARE ALLOWED
    while (isalpha(lex_current(lexer)))
    {
        lex_advance_len_inc(lexer);
    }
    lexer->index -= lexer->len;
    // NOTE(5717): check if needed to specify the type
    return lex_add_token(lexer, Tkn_BuiltinFunc);
}

inline void
lex_advance(Lexer *lexer)
{
    const char c = lex_current(lexer);
    lexer->index += 1;
    lexer->line += (c == '\n');
}

inline void
lex_advance_len_times(Lexer *lexer)
{
    lexer->index += lexer->len;
}

inline void
lex_advance_len_inc(Lexer *lexer)
{
    const char c = lex_current(lexer);
    lexer->index += 1;
    lexer->len += 1;
    lexer->line += (c == '\n');
}

inline char
lex_peek(Lexer *lexer)
{
    return lexer->file->contents[lexer->index + 1];
}

inline char
lex_current(Lexer *lexer)
{
    return lexer->file->contents[lexer->index];
}

inline char
lex_past(Lexer *lexer)
{
    return lexer->file->contents[lexer->index - 1];
}

inline bool
lex_is_not_eof(Lexer *lexer)
{
    return lexer->index < lexer->file_length;
}

inline bool
lex_keyword_match(Lexer *lexer, cstr cstr, uint length)
{
    return strncmp(lexer->file->contents + lexer->index, cstr, length) == 0;
}

void
lex_save_state(Lexer *lexer)
{
    lexer->save_index = lexer->index;
    lexer->save_line  = lexer->line;
}

void
lex_restore_state_for_err(Lexer *lexer)
{
    lexer->index = lexer->save_index;
    lexer->line  = lexer->save_line;
}

u8
lex_report_error(Lexer *lexer)
{
    //
    uint low = lexer->index, col = 0;
    const uint line  = lexer->line;
    const uint len   = lexer->len;
    const uint index = lexer->index;
    file_t *file     = lexer->file;
    while (file->contents[low] != '\n' && low > 0)
    {
        low--;
        col++;
    }
    low = low > 1 ? low + 1 : 0;

    //
    uint _length = lexer->index;
    while (file->contents[_length] != '\n' && _length + 1 < file->length)
        _length++;

    _length -= low;

    // error msg
    fprintf(stderr, " > %s%s%s:%u:%u: %serror: %s%s%s\n", BOLD, WHITE, file->name, line, col, LRED,
            LBLUE, lexer_err_msg(lexer->error), RESET);

    // line from source code
    fprintf(stderr, "  %s%u%s | %.*s\n", LYELLOW, line, RESET, _length, (file->contents + low));

    const uint num_line_digits = get_digits_from_number(line);

    // arrows pointing to error location
    const uint spaces = index - low + 1;

    const uint MAX_ARROW_LEN = 101;
    if (len < 101)
    {
        char arrows[MAX_ARROW_LEN];
        memset(arrows, 0, MAX_ARROW_LEN);
        memset(arrows, '^', len);
        arrows[len] = '\0';

        fprintf(stderr, "  %*c |%*c%s%s%s\n", num_line_digits, ' ', spaces, ' ', LRED, BOLD,
                arrows);
    }
    else
    {
        fprintf(stderr, "  %*c |%*c%s%s^^^---...\n", num_line_digits, ' ', spaces, ' ', LRED, BOLD);
    }
    // error lexer_err_advice
    fprintf(stderr, " > Advice: %s%s\n", RESET, lexer_err_advice(lexer->error));
    return FAILURE;
}

u8
lex_add_token(Lexer *lexer, TknType type)
{
    // index at the end of the token
    Token tkn = (Token){lexer->index, lexer->len, lexer->begin_tkn_line, type};
    arr_push(lexer->tokens, tkn);
    lex_advance_len_times(lexer); // TODO: Test optimization
    return SUCCESS;
}
