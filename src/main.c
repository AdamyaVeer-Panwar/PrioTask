#include <stdio.h>
#include "linked_list.h"

int main(void)
{
    LinkedList list;

    initLinkedList(&list);

    Task t1 = {
        101,
        "DBMS Assignment",
        "DBMS",
        3,
        "15-10-2026",
        TASK_PENDING
    };

    Task t2 = {
        102,
        "ML Practical",
        "Machine Learning",
        5,
        "16-10-2026",
        TASK_PENDING
    };

    // INSERT
    insertTask(&list, t1);
    insertTask(&list, t2);

    // DISPLAY
    printf("\n--- ALL TASKS ---\n");
    displayTasks(&list);

    // SEARCH
    printf("\n--- SEARCH ---\n");

    Task *found = searchTask(&list, 102);

    if (found != NULL) {
        printf("Found: %s\n", found->name);
    } else {
        printf("Task not found.\n");
    }

    // UPDATE
    printf("\n--- UPDATE ---\n");

    if (updateTask(&list, 102)) {
        printf("Task updated successfully.\n");
    } else {
        printf("Task not found.\n");
    }

    // DISPLAY AFTER UPDATE
    printf("\n--- AFTER UPDATE ---\n");
    displayTasks(&list);

    // DELETE
    printf("\n--- DELETE ---\n");

    if (deleteTask(&list, 101)) {
        printf("Task deleted successfully.\n");
    } else {
        printf("Task not found.\n");
    }

    // DISPLAY AFTER DELETE
    printf("\n--- AFTER DELETE ---\n");
    displayTasks(&list);

    // FREE MEMORY — MUST BE LAST
    freeLinkedList(&list);

    return 0;
}