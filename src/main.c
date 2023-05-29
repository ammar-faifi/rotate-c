#include "include/common.h"
#include "include/compile.h"

cstr
main_err(Stage s)
{
    switch (s)
    {
        case ST_FILE: return "FILE READ";
        case ST_LEXER: return "LEXER";
        case ST_PARSER: return "PARSER";
        case ST_TCHECKER: return "TYPE CHECKER";
        case ST_LOGGER: return "LOGGER";
        default: return "UNKNOWN";
    }
    return "UNKNOWN";
}

int
main(const int argc, char **const argv)
{

    if (argc < 2)
    {
        print_version_and_exit();
        return SUCCESS;
    }

    // parse program arguments
    compile_options comp_opt = compile_options_new(argc, argv);

    // setup timer stuff
    clock_t start_t, end_t;
    f128 total_t;
    start_t = clock();

    // compile
    u8 _exit = compile(&comp_opt);
    if (_exit == FAILURE)
        log_stage(main_err(comp_opt.st));
    else if (_exit == SUCCESS)
        log_info("SUCCESS");

    // print comptime
    end_t   = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("[%sTIME%s] : %.5Lf sec\n", LMAGENTA, RESET, total_t);
    return SUCCESS;
}
