
#include "taskobject.h"

// =============================================================================
TaskObject::TaskObject(QObject* parent) :
    QObject(parent),
    Task()
{

}

// =============================================================================
TaskObject::TaskObject(const Task& task, QObject* parent) :
    QObject(parent),
    Task(task)
{

}

// =============================================================================
