
#include "dbmanager.h"

#include <QDebug>

#include "dbexception.h"

#define EXCEPTION_NOT_IMPLEMENTED()                         \
    DBException(                                            \
        QSqlError(                                          \
            "Unknown driver",                               \
            QString("%1 is not implemented").arg(__func__), \
            QSqlError::ErrorType::UnknownError              \
        ),                                                  \
        __FILE__,                                           \
        __LINE__                                            \
    )                                                       \

// =============================================================================
DBManager::DBManager(const QString& hostName, int port) :
    m_hostName(hostName),
    m_port(port)
{

}

// =============================================================================
void DBManager::createNewDB(const QString& dbname)
{
    Q_UNUSED(dbname);

    throw EXCEPTION_NOT_IMPLEMENTED();
};

// =============================================================================
void DBManager::checkExistDb(const QString& dbname)
{
    Q_UNUSED(dbname);

    throw EXCEPTION_NOT_IMPLEMENTED();
}

// =============================================================================
void DBManager::createNewEmployee(const QString& dbname,
                                  const Employee& employee)
{
    Q_UNUSED(dbname);
    Q_UNUSED(employee);

    throw EXCEPTION_NOT_IMPLEMENTED();
}

// =============================================================================
void DBManager::deleteEmployee(const QString& dbname, qint32 id)
{
    Q_UNUSED(dbname);
    Q_UNUSED(id);

    throw EXCEPTION_NOT_IMPLEMENTED();
}

// =============================================================================
void DBManager::changeEmployee(const QString& dbname,
                               const Employee& employee)
{
    Q_UNUSED(dbname);
    Q_UNUSED(employee);

    throw EXCEPTION_NOT_IMPLEMENTED();
}

// =============================================================================
QList<Employee> DBManager::getEmployees(const QString& dbname)
{
    Q_UNUSED(dbname);

    throw EXCEPTION_NOT_IMPLEMENTED();
}

// =============================================================================
void DBManager::createNewTask(const QString& dbname, const Task& task)
{
    Q_UNUSED(dbname);
    Q_UNUSED(task);

    throw EXCEPTION_NOT_IMPLEMENTED();
}

// =============================================================================
QList<Task> DBManager::getTasks(const QString& dbname)
{
    Q_UNUSED(dbname);

    throw EXCEPTION_NOT_IMPLEMENTED();
}

// =============================================================================
void DBManager::deleteTask(const QString& dbname, qint32 id)
{
    Q_UNUSED(dbname);
    Q_UNUSED(id);

    throw EXCEPTION_NOT_IMPLEMENTED();
}

// =============================================================================
void DBManager::changeTask(const QString& dbname, const Task& task)
{
    Q_UNUSED(dbname);
    Q_UNUSED(task);

    throw EXCEPTION_NOT_IMPLEMENTED();
}

// =============================================================================
