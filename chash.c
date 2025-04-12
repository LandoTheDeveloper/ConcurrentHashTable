// Main File

#include "hash_table.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  hash_struct *table;
  char *name;
  int salary;
} threadInput;

int main() {
  hash_struct *table = create_table();
  int inputNum = 7;
  setInsertCounter(4);

  insert(table, "Alice", 50000); // Insert Alice
  delete (table, "Alice");
  insert(table, "Bob", 60000);     // Insert Bob
  insert(table, "Charlie", 70000); // Insert Charlie
  insert(table, "Alice", 55000);   // Update Alice's salary
  print_table(table);
  search(table, "Bob");
  search(table, "Alice");
  print_table(table);

  pthread_t *threads = malloc(inputNum * sizeof(pthread_t));

  for (int i = 0; i < inputNum; ++i) {

    if (threadTypes[i] == 0) {
      pthread_create(&threads[i], NULL, insert, (void *)inputs[i]);
    }

    if (threadTypes[i] == 1) {
      pthread_create(&threads[i], NULL, delete, (void *)inputs[i]);
    }

    if (threadTypes[i] == 2) {
      pthread_create(&threads[i], NULL, search, (void *)inputs[i]);
    }
  }

  for (int i = 0; i < inputNum; ++i) {

    pthread_join(threads[i], NULL);
  }

  return 0;
}
