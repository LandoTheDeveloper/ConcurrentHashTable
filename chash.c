// Main File

#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    hash_struct *table = create_table();

    delete (table, "John Doe");

    return 0;
}