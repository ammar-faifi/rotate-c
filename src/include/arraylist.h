#pragma once

#include "defines.h"

// General sequence macros
#define seq_elem_type(seq) typeof(*(seq)->elements)

#define seq_start(seq) ((seq)->elements)
#define seq_end(seq)   ((seq)->elements + (seq)->count)

#define seq_len(seq) ((seq)->count)

#define make_slice(T, start, count) ((slice(T)){start, count})
#define arr_slice(arr, low, high)                                                                  \
    {                                                                                              \
        (high) - (low), arr_start(arr) + (low)                                                     \
    }

#define for_each(seq, iter)                                                                        \
    for (seq_elem_type(seq) *iter = seq_start(seq); iter < seq_end(seq); iter++)

// Slice type

#define slice(T) slice_##T

#define decl_slice(T)                                                                              \
    typedef struct                                                                                 \
    {                                                                                              \
        size_t count;                                                                              \
        T *elements;                                                                               \
    } slice(T)

// arraylist type

#define ARRAY_GROWTH_FACTOR 2
#define arr(T)              arraylist_##T
#define decl_arr(T)                                                                                \
    typedef struct T                                                                               \
    {                                                                                              \
        usize count;                                                                               \
        usize capacity;                                                                            \
        T elements[];                                                                              \
    } * arr(T);                                                                                    \
    decl_slice(T)

// arraylist implementation

#define arr_total_size(arr)                                                                        \
    (sizeof(_arraylist_header) + (arr)->capacity * sizeof(seq_elem_type(arr)))

typedef struct
{
    usize count;
    usize capacity;
} _arraylist_header;

static inline _arraylist_header *
_new_arr(_arraylist_header *header, usize initial_size)
{
    header->count    = 0;
    header->capacity = initial_size;
    return header;
}

#define make_arr(T, size)                                                                          \
    ((arr(T))_new_arr(mem_alloc(sizeof(_arraylist_header) + size * sizeof(T)), size))
#define arr_free(arr) mem_free(arr)

#define arr_push(arr, ...)                                                                         \
    do                                                                                             \
    {                                                                                              \
        if ((arr)->count + 1 > (arr)->capacity)                                                    \
        {                                                                                          \
            (arr)->capacity <<= 1;                                                                 \
            (arr) = mem_resize(arr, arr_total_size(arr));                                          \
        }                                                                                          \
        (arr)->elements[(arr)->count++] = __VA_ARGS__;                                             \
    } while (0);

#define arr_start(arr) seq_start(arr)
#define arr_end(arr)   seq_end(arr)
// WARN: no bounds checking
#define arr_at(arr, idx) (arr)->elements[(idx)]

#define arr_len(arr) seq_len(arr)

#define arr_foreach(arr, el) for_each(arr, el)

// end of arr.H
