#include "include/compile.h"
#include "include/common.h"
#include "include/file.h"

#include "include/log.h"

#include "fe/parser.h"

u8
compile(compile_options *options)
{

    // Parser *parser;
    u8 exit = 0;

    // Read file
    options->st = ST_FILE;
    file_t file = file_read(options->filename);
    ASSERT_RET_FAIL(file.valid_code == success, "file read error");

    /*
     *
     * LEXICAL ANALYSIS
     *
     * */
    options->st = ST_LEXER;
    Lexer lexer = lexer_init(&file);
    exit        = lexer_lex(&lexer);
    if (lexer_get_tokens(&lexer)->count < 2u) log_error("file is empty");
    if (exit == FAILURE) return FAILURE;
    // parse lexed tokens to Abstract Syntax tree

    /*
     *
     * PARSING
     *
     * */
    // options->st   = ST_PARSER;
    // Parser parser = Parser(&file, &lexer);
    // if (!options->lex_only)
    // {
    //     exit = parser.parse_lexer();
    //     ASSERT_RET_FAIL(exit != FAILURE, "Parser error");
    // }

    // log compiliation
    if (options->debug_info)
    {
        // LOGS ONLY DURING SUCCESS OF THE PREVIOUS STAGES
        options->st  = ST_LOGGER;
        FILE *output = fopen("output.org", "wb");
        if (output)
        {
            log_compilation(output, &file, &lexer);
            fclose(output);
        }
        else
        {
            log_error("Log failed");
        }
    }

    file_free(&file);
    lexer_deinit(&lexer);

    return exit;
}

compile_options
compile_options_new(const i32 argc, i8 **argv)
{
    compile_options co;
    co.argc          = argc;
    co.argv          = argv;
    co.debug_info    = false;
    co.debug_symbols = false;
    co.timer         = false;
    co.lex_only      = false;
    co.st            = ST_UNKNOWN;

    co.filename = argv[1];
    for (i32 i = 2; i < argc; i++)
    {
        cstr arg = argv[i];

        if (strcmp(arg, "--log") == 0)
        {
            co.debug_info = true;
        }
        else if (!strcmp(arg, "--version") || !strcmp(arg, "-v"))
        {
            print_version_and_exit();
        }
        else if (strcmp(arg, "--timer") == 0)
        {
            co.timer = true;
        }
        else if (strcmp(arg, "--lex") == 0)
        {
            co.lex_only = true;
        }
        else
        {
            log_error_unknown_flag(arg);
        }
    }

    return co;
}

void
print_version_and_exit()
{
    cstr out = " Rotate Compiler \n Version: %s\n"
               " --lex   for lexical analysis\n"
               " --log   for dumping compilation info as orgmode format in output.org\n"
               " https://github.com/Airbus5717/rotate.git"
               "\n";
    fprintf(stdout, out, RTVERSION);
    exit(0);
}