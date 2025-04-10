// Main File

#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    hash_struct *table = create_table();
    setInsertCounter(4);
    insert(table, "Alice", 50000);   // Insert Alice
    insert(table, "Bob", 60000);     // Insert Bob
    insert(table, "Charlie", 70000); // Insert Charlie
    insert(table, "Alice", 55000);   // Update Alice's salary
    print_table(table);
    search(table, "Alice"); 
    delete (table, "Alice"); // Delete Alice
    search(table, "Alice"); 
    print_table(table);

    return 0;
}
