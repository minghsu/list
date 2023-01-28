#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "list.h"

typedef struct
{
    void *data;
    int size;
} TEST_LIST_ITEM_T;

#define MODEL_STACK 1
#define MODEL_QUEUE 2

void get_crc32_tbl(void)
{
    unsigned int crcTable[256], crcTmp1;

    for (int i = 0; i < 256; i++)
    {
        crcTmp1 = i;
        for (int j = 8; j > 0; j--)
        {
            if (crcTmp1 & 1)
                crcTmp1 = (crcTmp1 >> 1) ^ 0xEDB88320L;
            else
                crcTmp1 >>= 1;
        }
        crcTable[i] = crcTmp1;

        if (i % 8 == 0)
        {
            printf("\n0x%x, ", crcTmp1);
        }
        else
        {
            printf("0x%x, ", crcTmp1);
        }
    }
    printf("\n");
}

int test_list(int model)
{
    unsigned int count = (rand() % 100) + 1;           // 1 ~ 1000
    unsigned int test_search_index = (rand() % count); // 0 ~ count-1
    int test_search_match_index = -1;
    // unsigned int count = 10;
    TEST_LIST_ITEM_T *test_item = NULL;
    TEST_LIST_ITEM_T test_search_item;

    printf("Count: %u, Test Index: %u\n", count, test_search_index);
    u_int64_t list = list_initial();

    // printf("Start Timestamp: %d\n", (int)time(NULL));
    //  for (unsigned int i = count; i < 0; i--)
    for (unsigned int i = 0; i < count; i++)
    {
        LIST_MALLOC(test_item, sizeof(TEST_LIST_ITEM_T));
        if (test_item)
        {
            test_item->size = (rand() % 1024) + 1; // 1 ~ 1024
            LIST_MALLOC(test_item->data, test_item->size);

            if (model == MODEL_QUEUE)
                list_push_tail(list, test_item, sizeof(TEST_LIST_ITEM_T));
            else
                list_push_head(list, test_item, sizeof(TEST_LIST_ITEM_T));

            printf("ADD > Item: %p, Data: %p, Size: %d\n", test_item, test_item->data, test_item->size);

            if (i == test_search_index)
            {
                memset(&test_search_item, 0, sizeof(TEST_LIST_ITEM_T));
                memcpy(&test_search_item, test_item, sizeof(TEST_LIST_ITEM_T));
            }
        }
    }
    // printf("End Timestamp: %d\n", (int)time(NULL));

    // list_dump(list);

    printf("search item data: %p, size: %d\n", test_search_item.data, test_search_item.size);
    test_search_match_index = list_search(list, (void *)&test_search_item, sizeof(TEST_LIST_ITEM_T));
    printf("search result: %d\n", test_search_match_index);

    test_item = list_get_by_index(list, test_search_match_index);
    if (test_item)
    {
        printf("Data: %p (%u)\n", test_item->data, test_item->size);
    }

    // free test item
    test_item = list_pop_head(list);
    while (test_item)
    {
        // printf("REMOVE > Item: %p, Data: %p, Size: %d\n", test_item, test_item->data, test_item->size);

        LIST_FREE(test_item->data);
        LIST_FREE(test_item);
        test_item = list_pop_head(list);
    }

    list_destory(list);
}

int main(int argc, char *argv)
{
    srand(time(NULL));

    // get_crc32_tbl();
    printf("Test STACK model\n");
    test_list(MODEL_STACK);

    printf("\n\nTest QUEUE model\n");
    test_list(MODEL_QUEUE);
    return 0;
}