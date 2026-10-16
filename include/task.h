/* task.h */
#ifndef TASK_H
#define TASK_H

typedef enum {
    TASK_PENDING,
    TASK_QUEUED,
    TASK_COMPLETED
} TaskStatus;

typedef struct {
    int id;
    char name[100];
    char subject[50];
    int priority;
    char deadline[20];
    TaskStatus status;
} Task;

const char *statusToString(TaskStatus status);

#endif