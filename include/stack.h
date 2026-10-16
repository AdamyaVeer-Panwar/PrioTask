/* stack.h */
#ifndef STACK_H
#define STACK_H

typedef struct StackNode {
    int taskId;
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
} Stack;

void initStack(Stack *stack);
int isStackEmpty(const Stack *stack);
int push(Stack *stack, int taskId);
int pop(Stack *stack, int *taskId);
int peekStack(const Stack *stack, int *taskId);
void displayStack(const Stack *stack);
void freeStack(Stack *stack);

#endif