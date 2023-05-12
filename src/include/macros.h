#pragma once

// C standard library
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define local static

#ifndef nullptr
#define nullptr ((void *)(0))
#endif

#define ASSERT(expr, msg)                                                                          \
    do                                                                                             \
    {                                                                                              \
        if (!(expr))                                                                               \
        {                                                                                          \
            fprintf(stderr, "%sAssert failure: %s\n@file: %s:%d%s\n", LRED, msg, __FILE__,         \
                    __LINE__, RESET);                                                              \
            exit(1);                                                                               \
        }                                                                                          \
    } while (0)

#define ASSERT_CMP(expr1, expr2, msg)                                                              \
    do                                                                                             \
    {                                                                                              \
        if ((expr1) != (expr2))                                                                    \
        {                                                                                          \
            fprintf(stderr, "%sAssert failure: %s\n@file: %s:%d%s\n", LRED, msg, __FILE__,         \
                    __LINE__, RESET);                                                              \
            exit(1);                                                                               \
        }                                                                                          \
    } while (0)

#define ASSERT_NULL(expr, msg)                                                                     \
    do                                                                                             \
    {                                                                                              \
        if ((expr) == nullptr)                                                                     \
        {                                                                                          \
            fprintf(stderr, "%sAssert failure: %s\n@file: %s:%d%s\n", LRED, msg, __FILE__,         \
                    __LINE__, RESET);                                                              \
            exit(1);                                                                               \
        }                                                                                          \
    } while (0)

#define STR_ASSERT(string1, string2, msg)                                                          \
    do                                                                                             \
    {                                                                                              \
        if (strcmp(string1, string2) != 0)                                                         \
        {                                                                                          \
            fprintf(stderr, "%sAssert failure: %s\n@file: %s:%d%s\n", LRED, msg, __FILE__,         \
                    __LINE__, RESET);                                                              \
            exit(1);                                                                               \
        }                                                                                          \
    } while (0)

#define ASSERT_RET_FAIL(expr, msg)                                                                 \
    do                                                                                             \
    {                                                                                              \
        if (!(expr))                                                                               \
        {                                                                                          \
            log_error(msg);                                                                        \
            return EXIT_FAILURE;                                                                   \
        }                                                                                          \
    } while (0)

#define expect(expr, doWhenExpected, doElse)                                                       \
    do                                                                                             \
    {                                                                                              \
        if (!(expr))                                                                               \
        {                                                                                          \
            doElse;                                                                                \
            return EXIT_FAILURE;                                                                   \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            doWhenExpected;                                                                        \
        }                                                                                          \
    } while (0)

#ifdef DEBUG

#define dbg(str, ...)                                                                              \
    do                                                                                             \
    {                                                                                              \
        fprintf(stderr, str, __VA_ARGS__);                                                         \
    } while (0)

#endif

#define TODO(str)                                                                                  \
    do                                                                                             \
    {                                                                                              \
        fprintf(stderr, "%sTODO: %s\n@file: %s:%d%s\n", YELLOW, str, __FILE__, __LINE__, RESET);   \
        exit(1);                                                                                   \
    } while (0)

#define UNREACHABLE()                                                                              \
    do                                                                                             \
    {                                                                                              \
        fprintf(stderr, "%sREACHED UNREACHABLE STATE\n@file: %s:%d%s\n", LRED, __FILE__, __LINE__, \
                RESET);                                                                            \
        exit(1);                                                                                   \
    } while (0)

#define UNUSED(x) ((void)sizeof(x))

#if defined(_WIN32)
#define OS_WIN 1
#define OS_MAC 0
#define OS_LIN 0
#elif defined(__linux__)
#define OS_WIN 0
#define OS_MAC 0
#define OS_LIN 1
#elif defined(__APPLE__)
#define OS_WIN 0
#define OS_MAC 1
#define OS_LIN 0
#else
#error Missing OS detection
#endif
