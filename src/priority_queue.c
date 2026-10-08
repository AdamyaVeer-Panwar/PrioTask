#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"

void initPriorityQueue(PriorityQueue *pq)
{
    pq->front = NULL;
}

int isPriorityQueueEmpty(const PriorityQueue *pq)
{
    return pq->front == NULL;
}

void insertPriorityTask(
    PriorityQueue *pq,
    int taskId,
    int priority
)
{
    PriorityNode *newNode = malloc(sizeof(PriorityNode));

    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    newNode->taskId = taskId;
    newNode->priority = priority;
    newNode->next = NULL;

    if (pq->front == NULL ||
        priority > pq->front->priority) {

        newNode->next = pq->front;
        pq->front = newNode;
        return;
    }

    PriorityNode *current = pq->front;

    /*
     * Equal priorities remain in insertion order.
     */
    while (current->next != NULL &&
           current->next->priority >= priority) {

        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
}

int removeHighestPriority(PriorityQueue *pq)
{
    if (isPriorityQueueEmpty(pq)) {
        return -1;
    }

    PriorityNode *temp = pq->front;
    int taskId = temp->taskId;

    pq->front = pq->front->next;

    free(temp);

    return taskId;
}

int peekHighestPriority(const PriorityQueue *pq)
{
    if (isPriorityQueueEmpty(pq)) {
        return -1;
    }

    return pq->front->taskId;
}

void displayPriorityQueue(const PriorityQueue *pq)
{
    if (isPriorityQueueEmpty(pq)) {
        printf("Priority Queue is empty.\n");
        return;
    }

    const PriorityNode *current = pq->front;

    while (current != NULL) {

        printf(
            "%d(P%d)",
            current->taskId,
            current->priority
        );

        if (current->next != NULL) {
            printf(" -> ");
        }

        current = current->next;
    }

    printf("\n");
}

void freePriorityQueue(PriorityQueue *pq)
{
    while (!isPriorityQueueEmpty(pq)) {
        removeHighestPriority(pq);
    }
}