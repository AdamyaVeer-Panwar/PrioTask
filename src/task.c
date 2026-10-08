#include "task.h"

const char *statusToString(TaskStatus status)
{
    switch (status) {
        case TASK_PENDING:
            return "PENDING";

        case TASK_QUEUED:
            return "QUEUED";

        case TASK_COMPLETED:
            return "COMPLETED";

        default:
            return "UNKNOWN";
    }
}