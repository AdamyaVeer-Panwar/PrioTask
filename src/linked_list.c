#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

void initLinkedList(LinkedList *list)
{
    if (list == NULL) {
        return;
    }

    list->head = NULL;
}

int insertTask(LinkedList *list, const Task *task)
{
    if (list == NULL || task == NULL) {
        return 0;
    }

    TaskNode *newNode = malloc(sizeof *newNode);
    if (newNode == NULL) {
        return 0;
    }

    /* Task is stored by value; the list owns only this node allocation. */
    newNode->task = *task;
    newNode->next = NULL;

    if (list->head == NULL) {
        list->head = newNode;
        return 1;
    }

    TaskNode *current = list->head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
    return 1;
}

Task *searchTask(LinkedList *list, int taskId)
{
    if (list == NULL) {
        return NULL;
    }

    for (TaskNode *current = list->head; current != NULL; current = current->next) {
        if (current->task.id == taskId) {
            return &current->task;
        }
    }

    return NULL;
}

int deleteTask(LinkedList *list, int taskId)
{
    if (list == NULL) {
        return 0;
    }

    TaskNode *previous = NULL;
    TaskNode *current = list->head;
    while (current != NULL) {
        if (current->task.id == taskId) {
            if (previous == NULL) {
                list->head = current->next;
            } else {
                previous->next = current->next;
            }

            free(current);
            return 1;
        }

        previous = current;
        current = current->next;
    }

    return 0;
}

int updateTask(LinkedList *list, int taskId, const Task *updatedTask)
{
    Task *task = searchTask(list, taskId);
    if (task == NULL || updatedTask == NULL || updatedTask->id != taskId) {
        return 0;
    }

    *task = *updatedTask;
    return 1;
}

void displayTasks(const LinkedList *list)
{
    if (list == NULL || list->head == NULL) {
        printf("No tasks available.\n");
        return;
    }

    for (const TaskNode *current = list->head; current != NULL; current = current->next) {
        printf(
            "ID: %d | Name: %s | Subject: %s | Priority: %d | Deadline: %s | Status: %s\n",
            current->task.id,
            current->task.name,
            current->task.subject,
            current->task.priority,
            current->task.deadline,
            statusToString(current->task.status)
        );
    }
}

void freeLinkedList(LinkedList *list)
{
    if (list == NULL) {
        return;
    }

    TaskNode *current = list->head;
    while (current != NULL) {
        TaskNode *next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
}
