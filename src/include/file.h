#pragma once

#include "common.h"

typedef enum
{
    success,
    failure,
} valid;

typedef struct
{
    cstr name;
    cstr contents;
    const uint length; // contents length
    valid valid_code;
} file_t;

file_t file_read(cstr name);
void file_free(file_t *);
