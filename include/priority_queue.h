#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

typedef struct PriorityNode {
    int taskId;
    int priority;
    struct PriorityNode *next;
} PriorityNode;

typedef struct {
    PriorityNode *front;
} PriorityQueue;

void initPriorityQueue(PriorityQueue *pq);

int isPriorityQueueEmpty(const PriorityQueue *pq);

void insertPriorityTask(
    PriorityQueue *pq,
    int taskId,
    int priority
);

int removeHighestPriority(PriorityQueue *pq);

int peekHighestPriority(const PriorityQueue *pq);

void displayPriorityQueue(const PriorityQueue *pq);

void freePriorityQueue(PriorityQueue *pq);

#endif