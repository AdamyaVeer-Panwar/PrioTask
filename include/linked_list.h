/* linked_list.h */
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "task.h"

typedef struct TaskNode {
    Task task;
    struct TaskNode *next;
} TaskNode;

typedef struct {
    TaskNode *head;
} LinkedList;

void initLinkedList(LinkedList *list);
int insertTask(LinkedList *list, const Task *task);
Task *searchTask(LinkedList *list, int taskId);
int deleteTask(LinkedList *list, int taskId);
int updateTask(LinkedList *list, int taskId, const Task *updatedTask);
void displayTasks(const LinkedList *list);
void freeLinkedList(LinkedList *list);

#endif
