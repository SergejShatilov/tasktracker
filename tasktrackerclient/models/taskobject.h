
#pragma once

#include <QObject>
#include "task.h"

class TaskObject : public QObject, public Task
{
    Q_OBJECT

public:
    explicit TaskObject(QObject* parent = nullptr);
    explicit TaskObject(const Task& task,
                        QObject* parent = nullptr);

public:
    Q_PROPERTY(qint32 id READ id WRITE setId);
    Q_PROPERTY(QString name READ name WRITE setName);
    Q_PROPERTY(QString state READ stateString WRITE setStateString);
    Q_PROPERTY(qint32 executorId READ executorId WRITE setExecutorId);
    Q_PROPERTY(QDate start READ start WRITE setStart);
    Q_PROPERTY(qint32 duration READ duration WRITE setDuration);
    Q_PROPERTY(qint32 parentId READ parentId WRITE setParentId);
    Q_PROPERTY(QString description READ description WRITE setDescription);

};
