/* queue.h */
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
int enqueue(Queue *queue, int taskId);
int dequeue(Queue *queue, int *taskId);
int peekQueue(const Queue *queue, int *taskId);
void displayQueue(const Queue *queue);
void freeQueue(Queue *queue);

#endif