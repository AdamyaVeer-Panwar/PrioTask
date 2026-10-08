#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void initQueue(Queue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
}

int isQueueEmpty(const Queue *queue)
{
    return queue->front == NULL;
}

void enqueue(Queue *queue, int taskId)
{
    QueueNode *newNode = malloc(sizeof(QueueNode));

    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    newNode->taskId = taskId;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
        return;
    }

    queue->rear->next = newNode;
    queue->rear = newNode;
}

int dequeue(Queue *queue)
{
    if (isQueueEmpty(queue)) {
        return -1;
    }

    QueueNode *temp = queue->front;
    int taskId = temp->taskId;

    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);

    return taskId;
}

int peekQueue(const Queue *queue)
{
    if (isQueueEmpty(queue)) {
        return -1;
    }

    return queue->front->taskId;
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
    while (!isQueueEmpty(queue)) {
        dequeue(queue);
    }
}