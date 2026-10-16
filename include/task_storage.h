#ifndef TASK_STORAGE_H
#define TASK_STORAGE_H

#include "linked_list.h"

int loadTasks(LinkedList *list, const char *path);
int saveTasks(const LinkedList *list, const char *path);

#endif
