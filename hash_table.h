// Function prototypes

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <pthread.h>

#define TABLE_SIZE 1000

typedef struct Node {
    int key;
    struct Node* next;
} Node;

