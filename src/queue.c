#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void initQueue(Queue *queue)
{
    if (queue == NULL) {
        return;
    }

    queue->front = NULL;
    queue->rear = NULL;
}

int isQueueEmpty(const Queue *queue)
{
    return queue == NULL || queue->front == NULL;
}

int enqueue(Queue *queue, int taskId)
{
    if (queue == NULL) {
        return 0;
    }

    QueueNode *newNode = malloc(sizeof *newNode);
    if (newNode == NULL) {
        return 0;
    }

    newNode->taskId = taskId;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }

    return 1;
}

int dequeue(Queue *queue, int *taskId)
{
    if (queue == NULL || taskId == NULL || queue->front == NULL) {
        return 0;
    }

    QueueNode *node = queue->front;
    *taskId = node->taskId;
    queue->front = node->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(node);
    return 1;
}

int peekQueue(const Queue *queue, int *taskId)
{
    if (queue == NULL || taskId == NULL || queue->front == NULL) {
        return 0;
    }

    *taskId = queue->front->taskId;
    return 1;
}

void displayQueue(const Queue *queue)
{
    if (isQueueEmpty(queue)) {
        printf("Queue is empty.\n");
        return;
    }

    const QueueNode *current = queue->front;
    while (current != NULL) {
        printf("%d", current->taskId);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }

    printf("\n");
}

void freeQueue(Queue *queue)
{
    if (queue == NULL) {
        return;
    }

    QueueNode *current = queue->front;
    while (current != NULL) {
        QueueNode *next = current->next;
        free(current);
        current = next;
    }

    queue->front = NULL;
    queue->rear = NULL;
}
