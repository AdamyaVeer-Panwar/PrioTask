#include <stdio.h>

#include "queue.h"
#include "priority_queue.h"
#include "stack.h"

int main(void)
{
    Queue queue;
    PriorityQueue pq;
    Stack stack;

    initQueue(&queue);
    initPriorityQueue(&pq);
    initStack(&stack);

    printf("\n--- QUEUE ---\n");

    enqueue(&queue, 101);
    enqueue(&queue, 102);
    enqueue(&queue, 103);

    displayQueue(&queue);

    printf("Dequeue: %d\n", dequeue(&queue));
    printf("Peek: %d\n", peekQueue(&queue));

    printf("\n--- PRIORITY QUEUE ---\n");

    insertPriorityTask(&pq, 101, 2);
    insertPriorityTask(&pq, 102, 5);
    insertPriorityTask(&pq, 103, 3);
    insertPriorityTask(&pq, 104, 5);

    displayPriorityQueue(&pq);

    printf(
        "Remove highest: %d\n",
        removeHighestPriority(&pq)
    );

    printf(
        "Peek highest: %d\n",
        peekHighestPriority(&pq)
    );

    printf("\n--- STACK ---\n");

    push(&stack, 101);
    push(&stack, 102);
    push(&stack, 103);

    displayStack(&stack);

    printf("Pop: %d\n", pop(&stack));
    printf("Peek: %d\n", peekStack(&stack));

    freeQueue(&queue);
    freePriorityQueue(&pq);
    freeStack(&stack);

    return 0;
}