
/*
 * Абстрактный класс для работы с БД
 */

#pragma once

#include <QList>
#include "dbauthorization.h"
#include "employee.h"
#include "task.h"

class DBManager
{
public:
    explicit DBManager(const QString& hostName, int port);

    virtual QString dbtype() const = 0;

    virtual void createNewDB(const DBAuthorization& authorization,
                             const QString& dbname,
                             const QString& dbowner);

    virtual void checkExistDb(const DBAuthorization& authorization,
                              const QString& dbname);

    virtual void createNewEmployee(const DBAuthorization& authorization,
                                   const QString& dbname,
                                   const Employee& employee);

    virtual void deleteEmployee(const DBAuthorization& authorization,
                                const QString& dbname,
                                qint32 id);

    virtual QList<Employee> getEmployees(const DBAuthorization& authorization,
                                         const QString& dbname);

    virtual void changeEmployee(const DBAuthorization& authorization,
                                const QString& dbname,
                                const Employee& employee);

    virtual void createNewTask(const DBAuthorization& authorization,
                               const QString& dbname,
                               const Task& task);

    virtual QList<Task> getTasks(const DBAuthorization& authorization,
                                 const QString& dbname);

    virtual void deleteTask(const DBAuthorization& authorization,
                            const QString& dbname,
                            qint32 id);

    virtual void changeTask(const DBAuthorization& authorization,
                            const QString& dbname,
                            const Task& task);
protected:
    QString m_hostName;
    int     m_port;
};
