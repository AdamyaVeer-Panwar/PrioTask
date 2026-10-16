#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"

void initPriorityQueue(PriorityQueue *pq)
{
    if (pq == NULL) {
        return;
    }

    pq->front = NULL;
}

int isPriorityQueueEmpty(const PriorityQueue *pq)
{
    return pq == NULL || pq->front == NULL;
}

int insertPriorityTask(PriorityQueue *pq, int taskId, int priority)
{
    if (pq == NULL) {
        return 0;
    }

    PriorityNode *newNode = malloc(sizeof *newNode);
    if (newNode == NULL) {
        return 0;
    }

    newNode->taskId = taskId;
    newNode->priority = priority;
    newNode->next = NULL;

    if (pq->front == NULL || priority > pq->front->priority) {
        newNode->next = pq->front;
        pq->front = newNode;
        return 1;
    }

    PriorityNode *current = pq->front;
    /* Keep equal-priority tasks in their insertion order. */
    while (current->next != NULL && current->next->priority >= priority) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
    return 1;
}

int removeHighestPriority(PriorityQueue *pq, int *taskId)
{
    if (pq == NULL || taskId == NULL || pq->front == NULL) {
        return 0;
    }

    PriorityNode *node = pq->front;
    *taskId = node->taskId;
    pq->front = node->next;
    free(node);
    return 1;
}

int peekHighestPriority(const PriorityQueue *pq, int *taskId)
{
    if (pq == NULL || taskId == NULL || pq->front == NULL) {
        return 0;
    }

    *taskId = pq->front->taskId;
    return 1;
}

void displayPriorityQueue(const PriorityQueue *pq)
{
    if (isPriorityQueueEmpty(pq)) {
        printf("Priority Queue is empty.\n");
        return;
    }

    const PriorityNode *current = pq->front;
    while (current != NULL) {
        printf("%d(P%d)", current->taskId, current->priority);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

void freePriorityQueue(PriorityQueue *pq)
{
    if (pq == NULL) {
        return;
    }

    PriorityNode *current = pq->front;
    while (current != NULL) {
        PriorityNode *next = current->next;
        free(current);
        current = next;
    }

    pq->front = NULL;
}
