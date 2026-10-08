#ifndef QUEUE_H
#define QUEUE_H

typedef struct QueueNode {
    int taskId;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
} Queue;

void initQueue(Queue *queue);

int isQueueEmpty(const Queue *queue);

void enqueue(Queue *queue, int taskId);
int dequeue(Queue *queue);
int peekQueue(const Queue *queue);

void displayQueue(const Queue *queue);

void freeQueue(Queue *queue);

#endif