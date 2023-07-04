#ifndef MEM_H
#define MEM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
  void *ptr;
  char *name;
  size_t size;
  bool released;
  void *next;
} TrackedPointer;

void *tracked_malloc(size_t size, char *name);
void tracked_free(void *ptr);
void check_memory_leaks();

#endif
