#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define NULL_PTR_STR(p) (p) ? p : "NULL"

#if INTPTR_MAX == INT64_MAX
// 64-bit
typedef u_int64_t LIST_HANDLE;
typedef u_int64_t LIST_COUNT;
typedef u_int64_t LIST_SIZE;
typedef int64_t LIST_INDEX;
#elif INTPTR_MAX == INT32_MAX
typedef u_int32_t LIST_HANDLE;
typedef u_int32_t LIST_COUNT;
typedef u_int32_t LIST_SIZE;
typedef int32_t LIST_INDEX;
// 32-bit
#else
#error Unknown pointer size or missing size macros!
#endif

typedef enum
{
    LIST_ERR_NONE = 0,
    LIST_ERR_INVALID_HANDLE = -1,
    LIST_ERR_NO_MEMORY = -2,
    LIST_ERR_EMPTY = -3,
    LIST_ERR_INVALID_ARGUMENT = -4,
    LIST_ERR_NOT_FOUND = -5
} LIST_ERROR_E;

/**
 *  Memory ALLOC/FREE MACRO defined
 **/
#define LIST_MALLOC(p, s) p = list_malloc(s, __FILE__, __LINE__)
#define LIST_FREE(p) list_free(p, __FILE__, __LINE__)

/**
 *  To enable/disable the debug info for memory ALLOC/FREE status
 **/
#define LIST_MEMORY_DEBUG 0

/**
 *  CRC32 Data Length, 0 for full data
 *  <= : Calculate full data length
 *  >  : Calculate LIST_CRC32_MAX_BYTE length
 **/
#define LIST_CRC32_MAX_BYTE 64

typedef struct
{
    void *prev;
    void *next;
} LIST_INFO_T;

typedef struct
{
    void *data;
    LIST_SIZE size;
    u_int32_t hash;
} DATA_INFO_T;

typedef struct
{
    LIST_INFO_T list;
    DATA_INFO_T data;
} LIST_ITEM_T;

LIST_HANDLE list_initial(void);
LIST_ERROR_E list_destory(LIST_HANDLE a_lst_hnd);

// push & pop
LIST_ERROR_E list_push_head(LIST_HANDLE a_lst_hnd, void *ap_data, LIST_SIZE a_size);
LIST_ERROR_E list_push_tail(LIST_HANDLE a_lst_hnd, void *ap_data, LIST_SIZE a_size);
void *list_pop_head(LIST_HANDLE a_lst_hnd);
void *list_pop_tail(LIST_HANDLE a_lst_hnd);

// utility
LIST_COUNT list_count(LIST_HANDLE a_lst_hnd);
LIST_INDEX list_search(LIST_HANDLE a_lst_hnd, void *ap_data, LIST_SIZE a_size);
void *list_get_by_index(LIST_HANDLE a_lst_hnd, LIST_INDEX a_index);

// memory utility
void *list_malloc(LIST_SIZE a_size, int8_t *ap_file, LIST_INDEX a_line);
void list_free(void *ap_ptr, int8_t *ap_file, LIST_INDEX a_line);

// debug
void list_dump(LIST_HANDLE a_lst_hnd);
