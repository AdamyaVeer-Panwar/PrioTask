#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"
#include "priority_queue.h"
#include "queue.h"
#include "task_storage.h"

static void trimWhitespace(char *text)
{
    char *start = text;
    while (isspace((unsigned char)*start)) {
        ++start;
    }

    if (start != text) {
        memmove(text, start, strlen(start) + 1);
    }

    size_t length = strlen(text);
    while (length > 0 && isspace((unsigned char)text[length - 1])) {
        text[--length] = '\0';
    }
}

/*
 * Returns 1 for a valid line and 0 when input reaches EOF.
 * Empty input and overlong lines are rejected and requested again.
 */
static int readLine(const char *prompt, char *buffer, size_t bufferSize)
{
    if (buffer == NULL || bufferSize < 2) {
        return 0;
    }

    for (;;) {
        printf("%s", prompt);
        if (fgets(buffer, (int)bufferSize, stdin) == NULL) {
            return 0;
        }

        size_t length = strlen(buffer);
        if (length > 0 && buffer[length - 1] == '\n') {
            buffer[--length] = '\0';
        } else if (!feof(stdin)) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* Discard the remainder of an overlong line. */
            }
            puts("Input is too long. Please try again.");
            continue;
        }

        trimWhitespace(buffer);
        if (buffer[0] == '\0') {
            puts("Input cannot be empty. Please try again.");
            continue;
        }

        return 1;
    }
}

static int readInteger(const char *prompt, int minimum, int maximum, int *value)
{
    char input[128];

    if (value == NULL || minimum > maximum) {
        return 0;
    }

    for (;;) {
        if (!readLine(prompt, input, sizeof input)) {
            return 0;
        }

        errno = 0;
        char *end = NULL;
        long parsed = strtol(input, &end, 10);
        if (errno == ERANGE || end == input || *end != '\0' ||
            parsed < minimum || parsed > maximum) {
            printf("Enter a whole number from %d to %d.\n", minimum, maximum);
            continue;
        }

        *value = (int)parsed;
        return 1;
    }
}

static int isValidDeadline(const char *deadline)
{
    if (deadline == NULL || strlen(deadline) != 10 ||
        deadline[2] != '-' || deadline[5] != '-') {
        return 0;
    }

    for (size_t i = 0; i < 10; ++i) {
        if (i != 2 && i != 5 && !isdigit((unsigned char)deadline[i])) {
            return 0;
        }
    }

    int day = (deadline[0] - '0') * 10 + (deadline[1] - '0');
    int month = (deadline[3] - '0') * 10 + (deadline[4] - '0');
    int year = (deadline[6] - '0') * 1000 +
               (deadline[7] - '0') * 100 +
               (deadline[8] - '0') * 10 +
               (deadline[9] - '0');

    if (year < 1 || month < 1 || month > 12) {
        return 0;
    }

    static const int daysInMonth[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };
    int maximumDay = daysInMonth[month - 1];
    if (month == 2 &&
        (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))) {
        maximumDay = 29;
    }

    return day >= 1 && day <= maximumDay;
}

static int readDeadline(char deadline[20])
{
    for (;;) {
        if (!readLine("Deadline (DD-MM-YYYY): ", deadline, 20)) {
            return 0;
        }
        if (isValidDeadline(deadline)) {
            return 1;
        }
        puts("Enter a real calendar date in DD-MM-YYYY format.");
    }
}

static int readTaskDetails(Task *task)
{
    if (task == NULL ||
        !readLine("Task name: ", task->name, sizeof task->name) ||
        !readLine("Subject: ", task->subject, sizeof task->subject) ||
        !readInteger("Priority (1-5): ", 1, 5, &task->priority) ||
        !readDeadline(task->deadline)) {
        return 0;
    }

    return 1;
}

static int persistTasks(const LinkedList *list)
{
    if (saveTasks(list, "tasks.txt")) {
        return 1;
    }

    fputs("Warning: could not save tasks.txt; changes may be lost on exit.\n", stderr);
    return 0;
}

static void printMenu(void)
{
    puts("\n===== PRIORITY TASK MANAGER =====");
    puts("1. Add task");
    puts("2. List tasks");
    puts("3. Search task by ID");
    puts("4. Update task");
    puts("5. Delete task");
    puts("6. Add pending task to FIFO queue");
    puts("7. Process next queued task");
    puts("8. Show FIFO queue");
    puts("9. Process highest-priority pending task");
    puts("10. Show pending tasks by priority");
    puts("0. Exit");
}

static int buildPriorityQueue(const LinkedList *tasks, PriorityQueue *pq)
{
    initPriorityQueue(pq);
    for (const TaskNode *node = tasks->head; node != NULL; node = node->next) {
        if (node->task.status == TASK_PENDING &&
            !insertPriorityTask(pq, node->task.id, node->task.priority)) {
            freePriorityQueue(pq);
            return 0;
        }
    }
    return 1;
}

static void processHighestPriorityTask(LinkedList *tasks)
{
    PriorityQueue pq;
    int taskId;
    if (!buildPriorityQueue(tasks, &pq)) {
        fputs("Could not prepare the priority queue because memory allocation failed.\n", stderr);
        return;
    }

    if (!removeHighestPriority(&pq, &taskId)) {
        puts("There are no pending tasks.");
    } else {
        Task *task = searchTask(tasks, taskId);
        task->status = TASK_COMPLETED;
        persistTasks(tasks);
        printf("Processed highest-priority task %d: %s (priority %d)\n",
               taskId, task->name, task->priority);
    }
    freePriorityQueue(&pq);
}

static void showTasksByPriority(const LinkedList *tasks)
{
    PriorityQueue pq;
    if (!buildPriorityQueue(tasks, &pq)) {
        fputs("Could not prepare the priority queue because memory allocation failed.\n", stderr);
        return;
    }
    displayPriorityQueue(&pq);
    freePriorityQueue(&pq);
}

static void queueTask(LinkedList *tasks, Queue *queue)
{
    int taskId;
    if (!readInteger("Task ID to queue: ", 1, INT_MAX, &taskId)) {
        return;
    }

    Task *task = searchTask(tasks, taskId);
    if (task == NULL || task->status != TASK_PENDING) {
        puts("Only an existing pending task can be queued.");
        return;
    }

    for (QueueNode *node = queue->front; node != NULL; node = node->next) {
        if (node->taskId == taskId) {
            puts("That task is already in the queue.");
            return;
        }
    }

    if (!enqueue(queue, taskId)) {
        fputs("Could not queue the task because memory allocation failed.\n", stderr);
        return;
    }
    printf("Task %d added to the FIFO queue.\n", taskId);
}

static void processNextQueuedTask(LinkedList *tasks, Queue *queue)
{
    int taskId;
    while (dequeue(queue, &taskId)) {
        Task *task = searchTask(tasks, taskId);
        if (task == NULL || task->status != TASK_PENDING) {
            continue; /* Skip tasks deleted or completed while queued. */
        }

        task->status = TASK_COMPLETED;
        persistTasks(tasks);
        printf("Processed task %d: %s\n", taskId, task->name);
        return;
    }
    puts("There are no tasks in the FIFO queue.");
}

static void addTask(LinkedList *list)
{
    Task task = {0};

    if (!readInteger("Task ID (positive number): ", 1, INT_MAX, &task.id)) {
        return;
    }

    if (searchTask(list, task.id) != NULL) {
        puts("That task ID already exists.");
        return;
    }

    if (!readTaskDetails(&task)) {
        return;
    }

    task.status = TASK_PENDING;
    if (!insertTask(list, &task)) {
        fputs("Could not add the task because memory allocation failed.\n", stderr);
        return;
    }

    persistTasks(list);
    puts("Task added successfully.");
}

static void searchTaskById(LinkedList *list)
{
    int taskId;
    if (!readInteger("Task ID to search: ", 1, INT_MAX, &taskId)) {
        return;
    }

    const Task *task = searchTask(list, taskId);
    if (task == NULL) {
        puts("Task not found.");
        return;
    }

    printf(
        "ID: %d | Name: %s | Subject: %s | Priority: %d | Deadline: %s | Status: %s\n",
        task->id,
        task->name,
        task->subject,
        task->priority,
        task->deadline,
        statusToString(task->status)
    );
}

static void updateTaskById(LinkedList *list)
{
    int taskId;
    if (!readInteger("Task ID to update: ", 1, INT_MAX, &taskId)) {
        return;
    }

    Task *existing = searchTask(list, taskId);
    if (existing == NULL) {
        puts("Task not found.");
        return;
    }

    Task updated = *existing;
    if (!readTaskDetails(&updated)) {
        return;
    }

    if (!updateTask(list, taskId, &updated)) {
        puts("Task could not be updated.");
        return;
    }

    persistTasks(list);
    puts("Task updated successfully.");
}

static void deleteTaskById(LinkedList *list)
{
    int taskId;
    if (!readInteger("Task ID to delete: ", 1, INT_MAX, &taskId)) {
        return;
    }

    if (deleteTask(list, taskId)) {
        persistTasks(list);
        puts("Task deleted successfully.");
    } else {
        puts("Task not found.");
    }
}

int main(void)
{
    LinkedList tasks;
    Queue queue;
    initLinkedList(&tasks);
    initQueue(&queue);
    if (!loadTasks(&tasks, "tasks.txt")) {
        fputs("Could not load tasks.txt; check the file before continuing.\n", stderr);
        freeLinkedList(&tasks);
        return EXIT_FAILURE;
    }

    for (;;) {
        int choice;
        printMenu();
        if (!readInteger("Select an option: ", 0, 10, &choice)) {
            puts("\nInput closed. Exiting.");
            break;
        }

        switch (choice) {
            case 1:
                addTask(&tasks);
                break;
            case 2:
                displayTasks(&tasks);
                break;
            case 3:
                searchTaskById(&tasks);
                break;
            case 4:
                updateTaskById(&tasks);
                break;
            case 5:
                deleteTaskById(&tasks);
                break;
            case 6:
                queueTask(&tasks, &queue);
                break;
            case 7:
                processNextQueuedTask(&tasks, &queue);
                break;
            case 8:
                displayQueue(&queue);
                break;
            case 9:
                processHighestPriorityTask(&tasks);
                break;
            case 10:
                showTasksByPriority(&tasks);
                break;
            case 0:
                persistTasks(&tasks);
                puts("Goodbye.");
                freeQueue(&queue);
                freeLinkedList(&tasks);
                return 0;
        }
    }

    persistTasks(&tasks);
    freeQueue(&queue);
    freeLinkedList(&tasks);
    return 0;
}
