// Hash table logic (insert, delete, search)
#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

uint32_t jenkins_one_at_a_time_hash(const uint8_t *key, size_t length)
{
    size_t i = 0;
    uint32_t hash = 0;
    while (i != length)
    {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

hash_struct *create_table()
{
    hash_struct *table = malloc(sizeof(hash_struct) * TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        table[i].hash = 0;
        table[i].name[0] = '\0';
        table[i].salary = 0;
        table[i].next = NULL;
    }

    return table;
}
