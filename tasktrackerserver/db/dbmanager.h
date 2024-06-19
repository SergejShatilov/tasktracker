
/*
 * Абстрактный класс для работы с БД
 */

#pragma once

#include <QList>
#include <QSharedPointer>

#include "employee.h"
#include "task.h"

class DbManager
{
public:
    DbManager() = default;

    virtual QString dbType() const = 0;

    virtual void createDb(const QString& dbName) = 0;

    virtual void checkExistDb(const QString& dbName) = 0;

    virtual void createEmployee(const QString& dbName,
                                QSharedPointer<Employee> employee) = 0;

    virtual void deleteEmployee(const QString& dbName,
                                qint32 id) = 0;

    virtual void changeEmployee(const QString& dbName,
                                const QSharedPointer<Employee> employee) = 0;

    virtual QList<QSharedPointer<Employee>> getEmployees(
        const QString& dbName
    ) = 0;

    virtual void createTask(const QString& dbName,
                            QSharedPointer<Task> task) = 0;

    virtual void deleteTask(const QString& dbName,
                            qint32 id) = 0;

    virtual void changeTask(const QString& dbName,
                            const QSharedPointer<Task> task) = 0;

    virtual QList<QSharedPointer<Task>> getTasks(const QString& dbName) = 0;
};
