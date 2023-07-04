#include "mem.h"

TrackedPointer *records = NULL;

static TrackedPointer *_create_record(void *ptr, size_t size, char *name)
{
  TrackedPointer *record = (TrackedPointer *)malloc(sizeof(TrackedPointer));
  record->ptr = ptr;
  record->size = size;
  record->name = name;
  record->released = false;
  record->next = NULL;
  return record;
}

void *tracked_malloc(size_t size, char *name)
{
  void *ptr = malloc(size);

  if (records == NULL)
  {
    records = _create_record(ptr, size, name);
  }
  else
  {
    TrackedPointer *current = records;
    while (current->next != NULL)
      current = (TrackedPointer *)current->next;

    current->next = _create_record(ptr, size, name);
  }

  return ptr;
}

void tracked_free(void *ptr)
{
  free(ptr);
  TrackedPointer *current = records;
  bool found = false;

  while (current != NULL && !found)
  {
    if (current->ptr == ptr && !current->released)
    {
      found = true;
      current->released = true;
    }
    current = (TrackedPointer *)current->next;
  }

  if (!found)
    printf("Requested to free a record not found %p\n", ptr);
}

void check_memory_leaks()
{
  TrackedPointer *p = records;

  while (p != NULL)
  {
    if (!p->released)
    {
      printf("===>  [Error] %s pointer %p size %ld\n", p->name, p->ptr, p->size);
    }
    else
    {
      printf("[Ok] %s pointer %p size %ld\n", p->name, p->ptr, p->size);
    }

    p = (TrackedPointer *)p->next;
  }
}
