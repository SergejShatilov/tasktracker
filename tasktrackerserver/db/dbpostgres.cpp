
#include "dbpostgres.h"

#include <QSqlQuery>
#include "dbpostgresquery.h"
#include "dbopener.h"

// =============================================================================
DBPostgres::DBPostgres(const QString& hostName, int port) :
    m_hostName(hostName),
    m_port(port)
{

}

// =============================================================================
QString DBPostgres::dbtype() const {
    return QString("QPSQL");
}

// =============================================================================
void DBPostgres::createNewDB(const QString& dbname)
{
    QSqlDatabase db = addDatabase();
    DBPostgresQuery::createNewDB(db, dbname, "postgres");
}

// =============================================================================
void DBPostgres::checkExistDb(const QString& dbname)
{
    QSqlDatabase db = addDatabase();
    DBPostgresQuery::checkExistDb(db, dbname);
}

// =============================================================================
void DBPostgres::createNewEmployee(
    const QString& dbname,
    const Employee& employee)
{
    QSqlDatabase db = addDatabase();
    DBPostgresQuery::createNewEmployee(db, dbname, employee);
}

// =============================================================================
void DBPostgres::deleteEmployee(const QString& dbname, qint32 id)
{
    QSqlDatabase db = addDatabase();
    DBPostgresQuery::deleteEmployee(db, dbname, id);
}

// =============================================================================
QList<Employee> DBPostgres::getEmployees(const QString& dbname)
{
    QSqlDatabase db = addDatabase();
    return DBPostgresQuery::getEmployees(db, dbname);
}

// =============================================================================
void DBPostgres::changeEmployee(
    const QString& dbname,
    const Employee& employee)
{
    QSqlDatabase db = addDatabase();
    DBPostgresQuery::changeEmployee(db, dbname, employee);
}

// =============================================================================
Task DBPostgres::createNewTask(const QString& dbname, const Task& task)
{
    QSqlDatabase db = addDatabase();

    db.setDatabaseName(dbname);
    DBOpener opener(&db);

    QSqlQuery query(db);
    bool result = query.exec(
        QString(
            "INSERT INTO tasks ("
            "name, "
            "state, "
            "executor, "
            "start, "
            "duration, "
            "parent, "
            "description)\r\n"
            "VALUES ('%1', '%2', %3, '%4', %5, %6, '%7') "
            "RETURNING id;\r\n"
        )
        .arg(task.name())
        .arg(task.stateString())
        .arg(task.executorId())
        .arg(task.startString())
        .arg(task.duration())
        .arg(task.parentId())
        .arg(task.description())
    );

    if (!result) {
        throw DBException(query.lastError(), __FILE__, __LINE__);
    }

    if (!db.commit()) {
        throw DBException(db.lastError(), __FILE__, __LINE__);
    }

    Task taskResult(task);
    taskResult.setId(query.lastInsertId().toInt());

    return taskResult;
}

// =============================================================================
QList<Task> DBPostgres::getTasks(const QString& dbname)
{
    QSqlDatabase db = addDatabase();
    return DBPostgresQuery::getTasks(db, dbname);
}

// =============================================================================
void DBPostgres::deleteTask(const QString& dbname, qint32 id)
{
    QSqlDatabase db = addDatabase();
    DBPostgresQuery::deleteTask(db, dbname, id);
}

// =============================================================================
void DBPostgres::changeTask(const QString& dbname, const Task& task)
{
    QSqlDatabase db = addDatabase();
    DBPostgresQuery::changeTask(db, dbname, task);
}

// =============================================================================
QSqlDatabase
DBPostgres::addDatabase() const
{
    QSqlDatabase db = QSqlDatabase::addDatabase(dbtype());
    db.setHostName(m_hostName);
    db.setPort(m_port);
    db.setUserName("postgres");
    db.setPassword("123");

    return db;
}

// =============================================================================
