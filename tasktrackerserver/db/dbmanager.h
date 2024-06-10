
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
    explicit DBManager(const QString& hostName, int port);

    virtual QString dbtype() const = 0;

    virtual void createNewDB(const QString& dbname);

    virtual void checkExistDb(const QString& dbname);

    virtual void createNewEmployee(const QString& dbname,
                                   const Employee& employee);

    virtual void deleteEmployee(const QString& dbname,
                                qint32 id);

    virtual QList<Employee> getEmployees(const QString& dbname);

    virtual void changeEmployee(const QString& dbname,
                                const Employee& employee);

    virtual void createNewTask(const QString& dbname,
                               const Task& task);

    virtual QList<Task> getTasks(const QString& dbname);

    virtual void deleteTask(const QString& dbname,
                            qint32 id);

    virtual void changeTask(const QString& dbname,
                            const Task& task);
protected:
    QString m_hostName;
    int     m_port;
};
