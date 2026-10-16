#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void initStack(Stack *stack)
{
    if (stack == NULL) {
        return;
    }

    stack->top = NULL;
}

int isStackEmpty(const Stack *stack)
{
    return stack == NULL || stack->top == NULL;
}

int push(Stack *stack, int taskId)
{
    if (stack == NULL) {
        return 0;
    }

    StackNode *newNode = malloc(sizeof *newNode);
    if (newNode == NULL) {
        return 0;
    }

    newNode->taskId = taskId;
    newNode->next = stack->top;
    stack->top = newNode;
    return 1;
}

int pop(Stack *stack, int *taskId)
{
    if (stack == NULL || taskId == NULL || stack->top == NULL) {
        return 0;
    }

    StackNode *node = stack->top;
    *taskId = node->taskId;
    stack->top = node->next;
    free(node);
    return 1;
}

int peekStack(const Stack *stack, int *taskId)
{
    if (stack == NULL || taskId == NULL || stack->top == NULL) {
        return 0;
    }

    *taskId = stack->top->taskId;
    return 1;
}

void displayStack(const Stack *stack)
{
    if (isStackEmpty(stack)) {
        printf("Stack is empty.\n");
        return;
    }

    printf("TOP -> ");
    for (const StackNode *current = stack->top; current != NULL; current = current->next) {
        printf("%d", current->taskId);
        if (current->next != NULL) {
            printf(" -> ");
        }
    }
    printf("\n");
}

void freeStack(Stack *stack)
{
    if (stack == NULL) {
        return;
    }

    StackNode *current = stack->top;
    while (current != NULL) {
        StackNode *next = current->next;
        free(current);
        current = next;
    }

    stack->top = NULL;
}
