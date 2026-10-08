#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void initStack(Stack *stack)
{
    stack->top = NULL;
}

int isStackEmpty(const Stack *stack)
{
    return stack->top == NULL;
}

void push(Stack *stack, int taskId)
{
    StackNode *newNode = malloc(sizeof(StackNode));

    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    newNode->taskId = taskId;
    newNode->next = stack->top;

    stack->top = newNode;
}

int pop(Stack *stack)
{
    if (isStackEmpty(stack)) {
        return -1;
    }

    StackNode *temp = stack->top;
    int taskId = temp->taskId;

    stack->top = stack->top->next;

    free(temp);

    return taskId;
}

int peekStack(const Stack *stack)
{
    if (isStackEmpty(stack)) {
        return -1;
    }

    return stack->top->taskId;
}

void displayStack(const Stack *stack)
{
    if (isStackEmpty(stack)) {
        printf("Stack is empty.\n");
        return;
    }

    const StackNode *current = stack->top;

    printf("TOP -> ");

    while (current != NULL) {
        printf("%d", current->taskId);

        if (current->next != NULL) {
            printf(" -> ");
        }

        current = current->next;
    }

    printf("\n");
}

void freeStack(Stack *stack)
{
    while (!isStackEmpty(stack)) {
        pop(stack);
    }
}