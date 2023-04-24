#pragma once

#include "macros.h"

typedef float f32;
typedef double f64;
typedef long double f128; // !WARN: highly depends on the hardware

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef unsigned long long usize;
typedef signed long long isize;

static_assert(sizeof(i8) == 1, "invalid i8 size");
static_assert(sizeof(u8) == 1, "invalid u8 size");
static_assert(sizeof(i16) == 2, "invalid i16 size");
static_assert(sizeof(u16) == 2, "invalid u16 size");
static_assert(sizeof(i32) == 4, "invalid i32 size");
static_assert(sizeof(u32) == 4, "invalid u32 size");
static_assert(sizeof(i64) == 8, "invalid i64 size");
static_assert(sizeof(u64) == 8, "invalid u64 size");

static_assert(sizeof(f32) == 4, "invalid f32 size");
static_assert(sizeof(f64) == 8, "invalid f64 size");
static_assert(sizeof(f128) == 16, "invalid f128 size");

static_assert(sizeof(isize) == sizeof(i64), "invalid usize int size");
static_assert(sizeof(usize) == sizeof(u64), "invalid isize int size");

typedef const char *cstr;

#define SUCCESS 0u // EXIT_SUCCESS
#define FAILURE 1u // EXIT_FAILURE
#define DONE    2u // EXIT_DONE
static_assert(SUCCESS == 0 && EXIT_SUCCESS == SUCCESS, "Exit Success code is wrong");
static_assert(FAILURE == 1 && EXIT_FAILURE == FAILURE, "Exit Failure code is wrong");
static_assert(DONE == 2 && EXIT_FAILURE == (DONE - 1), "Exit Done code is wrong");

#if OS_WIN
#define NEWLINE "\n\r"
#else
#define NEWLINE "\n"
#endif

// terminal colors
#define RESET    "\x1b[0m"
#define BOLD     "\x1b[1m"
#define FAINT    "\x1b[2m"
#define GREEN    "\x1b[32m"
#define YELLOW   "\x1b[33m"
#define BLUE     "\x1b[34m"
#define PINK     "\x1b[35m"
#define CYAN     "\x1b[36m"
#define BLACK    "\x1b[30m"
#define WHITE    "\x1b[37m"
#define DEFAULT  "\x1b[39m"
#define LGRAY    "\x1b[90m"
#define LRED     "\x1b[91m"
#define LGREEN   "\x1b[92m"
#define LYELLOW  "\x1b[93m"
#define LBLUE    "\x1b[94m"
#define LMAGENTA "\x1b[95m"
#define LCYAN    "\x1b[96m"
#define LWHITE   "\x1b[97m"

// project specific
#define RTVERSION "0.0.1"
typedef u32 uint;
#define RUINT_MAX              UINT32_MAX
#define RUINT_MIN              0u
#define EXTRA_NULL_TERMINATORS 3u

static_assert(EXTRA_NULL_TERMINATORS > 2u, "keep the number above 2");
static_assert(RUINT_MIN == 0u, "Min number should unsigned 0");
static_assert(RUINT_MAX == UINT32_MAX, "Max number");
