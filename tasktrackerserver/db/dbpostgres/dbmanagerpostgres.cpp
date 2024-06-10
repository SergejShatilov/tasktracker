
#include "dbmanagerpostgres.h"
#include "dbpostgresquery.h"

// =============================================================================
DBManagerPostgres::DBManagerPostgres(const QString& hostName, int port) :
    DBManager(hostName, port)
{

}

// =============================================================================
QString DBManagerPostgres::dbtype() const {
    return QString("QPSQL");
}

// =============================================================================
void DBManagerPostgres::createNewDB(const QString& dbname)
{
    QSqlDatabase db = addDatabase();
    DBPostgresQuery::createNewDB(db, dbname, "postgres");
}

// =============================================================================
void DBManagerPostgres::checkExistDb(const QString& dbname)
{
    QSqlDatabase db = addDatabase();
    DBPostgresQuery::checkExistDb(db, dbname);
}

// =============================================================================
void DBManagerPostgres::createNewEmployee(
    const QString& dbname,
    const Employee& employee)
{
    QSqlDatabase db = addDatabase();
    DBPostgresQuery::createNewEmployee(db, dbname, employee);
}

// =============================================================================
void DBManagerPostgres::deleteEmployee(const QString& dbname, qint32 id)
{
    QSqlDatabase db = addDatabase();
    DBPostgresQuery::deleteEmployee(db, dbname, id);
}

// =============================================================================
QList<Employee> DBManagerPostgres::getEmployees(const QString& dbname)
{
    QSqlDatabase db = addDatabase();
    return DBPostgresQuery::getEmployees(db, dbname);
}

// =============================================================================
void DBManagerPostgres::changeEmployee(
    const QString& dbname,
    const Employee& employee)
{
    QSqlDatabase db = addDatabase();
    DBPostgresQuery::changeEmployee(db, dbname, employee);
}

// =============================================================================
void DBManagerPostgres::createNewTask(const QString& dbname, const Task& task)
{
    QSqlDatabase db = addDatabase();
    DBPostgresQuery::createNewTask(db, dbname, task);
}

// =============================================================================
QList<Task> DBManagerPostgres::getTasks(const QString& dbname)
{
    QSqlDatabase db = addDatabase();
    return DBPostgresQuery::getTasks(db, dbname);
}

// =============================================================================
void DBManagerPostgres::deleteTask(const QString& dbname, qint32 id)
{
    QSqlDatabase db = addDatabase();
    DBPostgresQuery::deleteTask(db, dbname, id);
}

// =============================================================================
void DBManagerPostgres::changeTask(const QString& dbname, const Task& task)
{
    QSqlDatabase db = addDatabase();
    DBPostgresQuery::changeTask(db, dbname, task);
}

// =============================================================================
QSqlDatabase
DBManagerPostgres::addDatabase() const
{
    QSqlDatabase db = QSqlDatabase::addDatabase(dbtype());
    db.setHostName(m_hostName);
    db.setPort(m_port);
    db.setUserName("postgres");
    db.setPassword("123");

    return db;
}

// =============================================================================
