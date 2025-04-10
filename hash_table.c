// Hash table logic (insert, delete, search)

/*
    TODO:
    insert - Bobby
    delete - Landon - DONE (need insert to test)
    search - Juan - DONE (Need to test)
    table create - Landon - DONE
    free table - Landon - DONE
*/

#include "hash_table.h"
#include "read_write_lock.h"
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

rwlock_t rwlock; // This is the read/write lock variable

// This will initially hold the total number of inserts. Each time an insert
// occurs, the counter will be decreased by 1. When it reaches 0, the deletes
// will be allowed to start
int insertCounter;

void setInsertCounter(int numberOfInserts) { insertCounter = numberOfInserts; }

uint32_t jenkins_one_at_a_time_hash(const uint8_t *key, size_t length) {
  size_t i = 0;
  uint32_t hash = 0;
  while (i != length) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

hash_struct *create_table() {
  hash_struct *table = malloc(sizeof(hash_struct) * TABLE_SIZE);
  for (int i = 0; i < TABLE_SIZE; i++) {
    table[i].hash = 0;
    table[i].name[0] = '\0';
    table[i].salary = 0;
    table[i].next = NULL;
  }

  rwlock_init(&rwlock);

  return table;
}

// insert will either update the table inputted with either a new entry or by
// replacing the information of an entry that matches it the inputs are a
// hash_struct* for the table to be updated, a char* for the name to be
// inputted, and an int for the salary to be inputted the output is nothing but
// it will update the table
void insert(hash_struct *table, char *name, uint32_t salary) {
  // getting the hash of the new node to be inputted and generate its index
  uint32_t hashOfNew =
      jenkins_one_at_a_time_hash((uint8_t *)name, strlen(name));
  uint32_t index = hashOfNew % TABLE_SIZE;

  // getting the entry of the table where the new data should be inputted
  hash_struct *originalData = &table[index];
  hash_struct *prev = NULL; // storage for last use value on the linked list
  int flag = 0;             // flag to see if an entry matched the input's hash

  rwlock_acquire_writelock(&rwlock);

  // checking to see if there has been a value inputted into this location in
  // the table before
  if (originalData->name[0] == '\0') {
    // No entry yet at this location, create a new entry
    originalData->hash = hashOfNew;
    strcpy(originalData->name, name);
    originalData->salary = salary;
    originalData->next = NULL;

    printf("Created new entry at index %u: %s with Salary %d\n", index, name,
           salary);
  } else {
    // looping through all of the nodes linked to the table entry
    while (originalData != NULL) {
      if (hashOfNew ==
          originalData
              ->hash) { // checking each hash value to see if there is a direct
                        // match and replacing the entry if there is
        strcpy(originalData->name, name);
        originalData->salary = salary;
        originalData->next = NULL;
        flag = 1;
        printf("Updated existing entry at index %u: %s with new Salary %d\n",
               index, name, salary);
        break;
      }
      prev = originalData;
      originalData = originalData->next;
    }

    // if an exact match was not found, the new data will be placed at the end
    if (flag == 0) {
      hash_struct *newNode =
          malloc(sizeof(hash_struct)); // creating a new table entry node
      newNode->hash = hashOfNew;
      strcpy(newNode->name, name);
      newNode->salary = salary;
      newNode->next = NULL;

      prev->next = newNode;
      printf("Created new node in linked list at index %u: %s with Salary %d\n",
             index, name, salary);
    }
  }
  rwlock_release_writelock(&rwlock);
  --insertCounter;
}


void delete(hash_struct *table, char *name) {

  while (insertCounter != 0) {
  }

  uint32_t hash = jenkins_one_at_a_time_hash((uint8_t *)name, strlen(name));
  uint32_t index = hash % TABLE_SIZE;

  rwlock_acquire_writelock(&rwlock); // Acquire write lock for delete operation

  hash_struct *current = &table[index];
  hash_struct *prev = NULL;

  if (current->name[0] == '\0') {
    printf("No entry found to delete at index %u.\n", index);
    rwlock_release_writelock(&rwlock); // Release the lock
    return;
  }

  if (strcmp(current->name, name) == 0) {
    if (current->next == NULL) {
      printf("Deleting %s from index %u (no next node).\n", name, index);
      current->hash = 0;
      current->name[0] = '\0';
      current->salary = 0;
    } else {
      hash_struct *temp = current->next;
      printf("Deleting %s from index %u, replacing with next node.\n", name,
             index);
      *current = *temp;
      free(temp);
    }
    rwlock_release_writelock(&rwlock); // Release the lock
    return;
  }

  prev = current;
  current = current->next;

  while (current != NULL) {
    if (strcmp(current->name, name) == 0) {
      printf("Deleting %s from index %u (found in linked list).\n", name,
             index);
      prev->next = current->next;
      free(current);
      rwlock_release_writelock(&rwlock); // Release the lock
      return;
    }
    prev = current;
    current = current->next;
  }

  printf("No entry with name %s found in the table.\n", name);
  rwlock_release_writelock(&rwlock); // Release the lock
}

hash_struct *search(hash_struct *table, char *name){
  uint32_t searchHash = jenkins_one_at_a_time_hash((uint8_t *)name, strlen(name));
  uint32_t index = searchHash % TABLE_SIZE;

  rwlock_acquire_readlock(&rwlock); // Acquire read lock for search operation

  hash_struct *current = &table[index];
  if (current->name[0] == '\0') {
    printf("No record found at %u.\n", index);
    rwlock_release_readlock(&rwlock); // Release the lock
    return NULL;
  }

  while (current != NULL) { //While loop finding the name in the linked list
    if (strcmp(current->name, name) == 0) {
      printf("Found %s at index %u with Salary %d.\n", name, index, current->salary);
      rwlock_release_readlock(&rwlock); // Release the lock
      return current;
    }
    current = current->next;
  }

  printf("No entry with name %s found in the table.\n", name);
  rwlock_release_readlock(&rwlock); // Release the lock
  return NULL;
}

void free_table(hash_struct *table) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    hash_struct *current = &table[i];
    while (current != NULL) {
      hash_struct *temp = current;
      current = current->next;
      free(temp);
    }
  }
  free(table);
}

void print_table(hash_struct *table) {
  printf("\n\nHash Table Contents:\n");
  for (int i = 0; i < TABLE_SIZE; i++) {
    hash_struct *current = &table[i];
    if (current->name[0] != '\0') {
      printf("Index %d: %s, Salary: %d\n", i, current->name, current->salary);
      while (current->next != NULL) {
        current = current->next;
        printf(" -> %s, Salary: %d\n", current->name, current->salary);
      }
    }
  }
}
