
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

    db.setDatabaseName(dbname);
    DBOpener opener(&db);

    QSqlQuery query(db);
    bool result = query.exec(
        QString(
            "SELECT * FROM tasks\r\n"
            "ORDER BY parent;\r\n"
        )
    );

    if (!result) {
        throw DBException(query.lastError(), __FILE__, __LINE__);
    }

    QList<Task> listTasks;
    while (query.next())
    {
        Task task;
        task.setId(query.value(0).toUInt());
        task.setName(query.value(1).toString());
        task.setStateString(query.value(2).toString());
        task.setExecutorId(query.value(3).toUInt());
        task.setStartString(query.value(4).toString());
        task.setDuration(query.value(5).toUInt());
        task.setParentId(query.value(6).toUInt());
        task.setDescription(query.value(7).toString());
        listTasks << task;
    }

    return listTasks;
}

// =============================================================================
void DBPostgres::deleteTask(const QString& dbname, qint32 id)
{
    QSqlDatabase db = addDatabase();

    db.setDatabaseName(dbname);
    DBOpener opener(&db);

    QSqlQuery query(db);
    bool result = query.exec(
        QString(
            "DELETE FROM tasks\r\n"
            "WHERE id=%1;\r\n"
        )
        .arg(id)
    );

    if (!result) {
        throw DBException(query.lastError(), __FILE__, __LINE__);
    }

    if (!db.commit()) {
        throw DBException(db.lastError(), __FILE__, __LINE__);
    }
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
