
#pragma once

#include <QString>
#include <QSqlDatabase>

#include "employee.h"
#include "task.h"

namespace DBPostgresQuery
{

void createNewDB(QSqlDatabase& db,
                 const QString& dbname,
                 const QString& owner);

void checkExistDb(QSqlDatabase& db,
                  const QString& dbname);

void createNewEmployee(QSqlDatabase& db,
                       const QString& dbname,
                       const Employee& employee);

void deleteEmployee(QSqlDatabase& db,
                    const QString& dbname,
                    qint32 id);

QList<Employee> getEmployees(QSqlDatabase& db,
                             const QString& dbname);

void changeEmployee(QSqlDatabase& db,
                    const QString& dbname,
                    const Employee& employee);

void deleteTask(QSqlDatabase& db,
                const QString& dbname,
                qint32 id);

void changeTask(QSqlDatabase& db,
                const QString& dbname,
                const Task& task);

}
