// Hash table logic (insert, delete, search)

/*
    TODO:
    insert - Bobby
    delete - Landon - DONE (need insert to test)
    search
    table create - Landon - DONE
    free table - Landon - DONE
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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

void delete(hash_struct *table, char *name)
{
    uint32_t hash = jenkins_one_at_a_time_hash((uint8_t *)name, strlen(name));
    uint32_t index = hash % TABLE_SIZE;

    hash_struct *current = &table[index];
    hash_struct *prev = NULL;

    // Check if head is empty
    if (current->name[0] == '\0')
        return;

    // If node matches
    if (strcmp(current->name, name) == 0)
    {
        // No next node, clear its data
        if (current->next == NULL)
        {
            current->hash = 0;
            current->name[0] = '\0';
            current->salary = 0;
        }
        else
        {
            // If next node exists, copy its data to current node
            hash_struct *temp = current->next;
            *current = *temp;
            free(temp);
        }
        return;
    }

    // Search linked list
    prev = current;
    current = current->next;

    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            prev->next = current->next;
            free(current);
            printf("Deleted: %s\n", name);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void free_table(hash_struct *table)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        hash_struct *current = &table[i];
        while (current != NULL)
        {
            hash_struct *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(table);
}