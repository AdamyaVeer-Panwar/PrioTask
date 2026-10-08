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

void push(Stack *stack, int taskId);
int pop(Stack *stack);
int peekStack(const Stack *stack);

void displayStack(const Stack *stack);

void freeStack(Stack *stack);

#endif