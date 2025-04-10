// Main File

#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    hash_struct *table = create_table();
    insert(table, "Alice", 50000);   // Insert Alice
    insert(table, "Bob", 60000);     // Insert Bob
    insert(table, "Charlie", 70000); // Insert Charlie
    insert(table, "Alice", 55000);   // Update Alice's salary
    delete (table, "Alice");         // Delete Alice

    return 0;
}