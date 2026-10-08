#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

void initLinkedList(LinkedList *list)
{
    list->head = NULL;
}

void insertTask(LinkedList *list, Task task)
{
    TaskNode *newNode = malloc(sizeof(TaskNode));

    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    newNode->task = task;
    newNode->next = NULL;

    if (list->head == NULL) {
        list->head = newNode;
        return;
    }

    TaskNode *current = list->head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newNode;
}

Task *searchTask(LinkedList *list, int taskId)
{
    TaskNode *current = list->head;

    while (current != NULL) {
        if (current->task.id == taskId) {
            return &current->task;
        }

        current = current->next;
    }

    return NULL;
}

int deleteTask(LinkedList *list, int taskId)
{
    if (list->head == NULL) {
        return 0;
    }

    TaskNode *current = list->head;
    TaskNode *previous = NULL;

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

int updateTask(LinkedList *list, int taskId)
{
    Task *task = searchTask(list, taskId);

    if (task == NULL) {
        return 0;
    }

    printf("Enter new task name: ");
    scanf(" %99[^\n]", task->name);

    printf("Enter new subject: ");
    scanf(" %49[^\n]", task->subject);

    printf("Enter new priority (1-5): ");
    scanf("%d", &task->priority);

    printf("Enter new deadline: ");
    scanf(" %19[^\n]", task->deadline);

    return 1;
}
void displayTasks(const LinkedList *list)
{
    if (list->head == NULL) {
        printf("No tasks available.\n");
        return;
    }

    const TaskNode *current = list->head;

    while (current != NULL) {
        printf(
            "ID: %d | Name: %s | Subject: %s | Priority: %d | Deadline: %s | Status: %s\n",
            current->task.id,
            current->task.name,
            current->task.subject,
            current->task.priority,
            current->task.deadline,
            statusToString(current->task.status)
        );

        current = current->next;
    }
}

void freeLinkedList(LinkedList *list)
{
    TaskNode *current = list->head;

    while (current != NULL) {
        TaskNode *next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
}