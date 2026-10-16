#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task_storage.h"

static int readStoredLine(FILE *file, char *buffer, size_t size)
{
    if (fgets(buffer, (int)size, file) == NULL) {
        return feof(file) ? 0 : -1;
    }

    size_t length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[--length] = '\0';
    } else {
        int ch = fgetc(file);
        if (ch != EOF) {
            while (ch != '\n' && ch != EOF) {
                ch = fgetc(file);
            }
            return -1;
        }
        if (ferror(file)) {
            return -1;
        }
    }

    if (length > 0 && buffer[length - 1] == '\r') {
        buffer[--length] = '\0';
    }
    return 1;
}

static int validDeadline(const char *date)
{
    if (date == NULL || strlen(date) != 10 ||
        date[2] != '-' || date[5] != '-') {
        return 0;
    }

    for (size_t i = 0; i < 10; ++i) {
        if (i != 2 && i != 5 && !isdigit((unsigned char)date[i])) {
            return 0;
        }
    }

    int day = (date[0] - '0') * 10 + date[1] - '0';
    int month = (date[3] - '0') * 10 + date[4] - '0';
    int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 +
               (date[8] - '0') * 10 + date[9] - '0';
    static const int monthLengths[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    if (year == 0 || month < 1 || month > 12) {
        return 0;
    }

    int maxDay = monthLengths[month - 1];
    if (month == 2 &&
        (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))) {
        maxDay = 29;
    }
    return day >= 1 && day <= maxDay;
}

static int parseRecordHeader(const char *line, int *id, int *priority, int *status)
{
    const char *cursor = line;
    long values[3];

    for (size_t i = 0; i < 3; ++i) {
        while (isspace((unsigned char)*cursor)) {
            ++cursor;
        }

        if (*cursor == '\0') {
            return 0;
        }

        errno = 0;
        char *end = NULL;
        values[i] = strtol(cursor, &end, 10);
        if (end == cursor || errno == ERANGE ||
            values[i] < 0 || values[i] > INT_MAX) {
            return 0;
        }
        cursor = end;
    }

    while (isspace((unsigned char)*cursor)) {
        ++cursor;
    }
    if (*cursor != '\0' || values[0] == 0) {
        return 0;
    }

    *id = (int)values[0];
    *priority = (int)values[1];
    *status = (int)values[2];
    return 1;
}

int saveTasks(const LinkedList *list, const char *path)
{
    if (list == NULL || path == NULL) {
        return 0;
    }

    FILE *file = fopen(path, "w");
    if (file == NULL) {
        return 0;
    }

    int success = fputs("PRIOTASKS 1\n", file) >= 0;
    for (const TaskNode *node = list->head; success && node != NULL;
         node = node->next) {
        const Task *task = &node->task;
        success = fprintf(file, "%d %d %d\n", task->id, task->priority,
                          (int)task->status) >= 0 &&
                  fprintf(file, "%s\n%s\n%s\n", task->name, task->subject,
                          task->deadline) >= 0;
    }

    if (fclose(file) != 0) {
        success = 0;
    }
    return success;
}

int loadTasks(LinkedList *list, const char *path)
{
    if (list == NULL || path == NULL || list->head != NULL) {
        return 0;
    }

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return errno == ENOENT;
    }

    LinkedList loaded;
    initLinkedList(&loaded);

    char line[128];
    int valid = readStoredLine(file, line, sizeof line) == 1 &&
                strcmp(line, "PRIOTASKS 1") == 0;

    while (valid) {
        int lineStatus = readStoredLine(file, line, sizeof line);
        if (lineStatus == 0) {
            break;
        }
        if (lineStatus < 0) {
            valid = 0;
            break;
        }

        int id;
        int priority;
        int status;
        if (!parseRecordHeader(line, &id, &priority, &status) ||
            priority < 1 || priority > 5 ||
            status < TASK_PENDING || status > TASK_COMPLETED) {
            valid = 0;
            break;
        }

        Task task = {0};
        task.id = id;
        task.priority = priority;
        task.status = (TaskStatus)status;

        if (readStoredLine(file, task.name, sizeof task.name) != 1 ||
            readStoredLine(file, task.subject, sizeof task.subject) != 1 ||
            readStoredLine(file, task.deadline, sizeof task.deadline) != 1 ||
            task.name[0] == '\0' || task.subject[0] == '\0' ||
            !validDeadline(task.deadline) ||
            searchTask(&loaded, task.id) != NULL ||
            !insertTask(&loaded, &task)) {
            valid = 0;
            break;
        }
    }

    if (ferror(file)) {
        valid = 0;
    }
    fclose(file);

    if (!valid) {
        freeLinkedList(&loaded);
        return 0;
    }

    list->head = loaded.head;
    return 1;
}
