// Function prototypes

#include <stdio.h>
#include <stdint.h>

#define TABLE_SIZE 1000

typedef struct hash_struct
{
    uint32_t hash;
    char name[50];
    uint32_t salary;
    struct hash_struct *next;
} hash_struct;

/*
    Field/Description:
    hash - 32-bit unsigned int for hash value produced by running name through Jenkins one at a time function.
    name - String up to len(50)
    salary - 32-bit unsigned
    next - pointer to next node in list
*/

void insert(hash_struct *table, char *name, uint32_t salary);
void delete(hash_struct *table, char *name);
hash_struct *search(hash_struct *table, char *name);
void print_table(hash_struct *table);
void free_table(hash_struct *table);
hash_struct *create_table();