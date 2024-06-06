
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
void DBManagerPostgres::createNewDB(
    const DBAuthorization& authorization,
    const QString& dbname,
    const QString& dbowner)
{
    QSqlDatabase db = addDatabase(authorization);
    DBPostgresQuery::createNewDB(db, dbname, dbowner);
}

// =============================================================================
void DBManagerPostgres::checkExistDb(
    const DBAuthorization& authorization,
    const QString& dbname)
{
    QSqlDatabase db = addDatabase(authorization);
    DBPostgresQuery::checkExistDb(db, dbname);
}

// =============================================================================
void DBManagerPostgres::createNewEmployee(
    const DBAuthorization& authorization,
    const QString& dbname,
    const Employee& employee)
{
    QSqlDatabase db = addDatabase(authorization);
    DBPostgresQuery::createNewEmployee(db, dbname, employee);
}

// =============================================================================
void DBManagerPostgres::deleteEmployee(
    const DBAuthorization& authorization,
    const QString& dbname,
    qint32 id)
{
    QSqlDatabase db = addDatabase(authorization);
    DBPostgresQuery::deleteEmployee(db, dbname, id);
}

// =============================================================================
QList<Employee> DBManagerPostgres::getEmployees(
    const DBAuthorization& authorization,
    const QString& dbname)
{
    QSqlDatabase db = addDatabase(authorization);
    return DBPostgresQuery::getEmployees(db, dbname);
}

// =============================================================================
void DBManagerPostgres::changeEmployee(
    const DBAuthorization& authorization,
    const QString& dbname,
    const Employee& employee)
{
    QSqlDatabase db = addDatabase(authorization);
    DBPostgresQuery::changeEmployee(db, dbname, employee);
}

// =============================================================================
QSqlDatabase
DBManagerPostgres::addDatabase(const DBAuthorization& authorization) const
{
    QSqlDatabase db = QSqlDatabase::addDatabase(dbtype());
    db.setHostName(m_hostName);
    db.setPort(m_port);
    db.setUserName(authorization.userName());
    db.setPassword(authorization.passWord());

    return db;
}

// =============================================================================
