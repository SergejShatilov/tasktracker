
/*
 * Абстрактный класс для работы с БД
 */

#pragma once

#include <QList>
#include "dbauthorization.h"
#include "employee.h"

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
protected:
    QString m_hostName;
    int     m_port;
};
