
/*
 * Абстрактный класс для работы с БД
 */

#pragma once

#include <QList>
#include "employee.h"
#include "task.h"

class DBManager
{
public:
    DBManager() = default;

    virtual QString dbtype() const = 0;

    virtual void createNewDB(const QString& dbname) = 0;

    virtual void checkExistDb(const QString& dbname) = 0;

    virtual void createNewEmployee(const QString& dbname,
                                   const Employee& employee) = 0;

    virtual void deleteEmployee(const QString& dbname,
                                qint32 id) = 0;

    virtual QList<Employee> getEmployees(const QString& dbname) = 0;

    virtual void changeEmployee(const QString& dbname,
                                const Employee& employee) = 0;

    virtual Task createNewTask(const QString& dbname,
                               const Task& task) = 0;

    virtual QList<Task> getTasks(const QString& dbname) = 0;

    virtual void deleteTask(const QString& dbname,
                            qint32 id) = 0;

    virtual void changeTask(const QString& dbname,
                            const Task& task) = 0;
};
