
#include "dbpostgresquery.h"

#include <QDebug>
#include <QSqlQuery>

#include "db/dbexception.h"
#include "db/dbopener.h"

namespace
{
    // =========================================================================
    // Инициализация таблицы сотрудников в уже имеющейся БД
    static void initTableEmployees_(QSqlDatabase& db)
    {
        qDebug() << "Init table employees...";
        QSqlQuery query(db);
        bool result = query.exec(
            QString(
                "CREATE TABLE employees\r\n"
                "(\r\n"
                "\tid SERIAL PRIMARY KEY,\r\n"
                "\tlastname CHARACTER VARYING(30),\r\n"     // Фамилия
                "\tfirstname CHARACTER VARYING(30),\r\n"    // Имя
                "\tpatronymic CHARACTER VARYING(30),\r\n"   // Отчество
                "\tposition CHARACTER VARYING(30),\r\n"     // Должность
                "\temail CHARACTER VARYING(30),\r\n"        // Почта
                "\tphone CHARACTER VARYING(20),\r\n"        // Телефон
                "\tUNIQUE(email, phone),\r\n"
                "\tCHECK((email != '') AND (phone != ''))\r\n"
                ");\r\n"
            )
        );

        if (!result) {
            throw DBException(query.lastError(), __FILE__, __LINE__);
        }
    }

    // =========================================================================
    // Инициализация таблицы задач в уже имеющейся БД
    static void initTableTasks_(QSqlDatabase& db)
    {
        qDebug() << "Init table tasks...";
        QSqlQuery query(db);
        bool result = query.exec(
            QString(
                "CREATE TABLE tasks\r\n"
                "(\r\n"
                "\tid SERIAL PRIMARY KEY,\r\n"
                "\tname CHARACTER VARYING(60),\r\n"             // Наименование задачи
                "\tstate task_state,\r\n"                       // Состояние задачи
                "\texecutor INTEGER,\r\n"                       // ID исполнителя
                "\tstart DATE,\r\n"                         // Дата начала
                "\tduration INTEGER,\r\n"                       // Длительность задачи в днях
                "\tparent INTEGER,\r\n"                         // ID родительской задачи
                "\tdescription TEXT\r\n"                       // Описание задачи
               // "\tFOREIGN KEY (executor) REFERENCES employees (id),\r\n"
                //"\tFOREIGN KEY (parent) REFERENCES tasks (id)\r\n"
                ");\r\n"
            )
        );

        if (!result) {
            throw DBException(query.lastError(), __FILE__, __LINE__);
        }
    }

    // =========================================================================
    // Создание перечисления для состояния задачи
    static void initEnumTaskState_(QSqlDatabase& db)
    {
        qDebug() << "Init enum task state...";
        QSqlQuery query(db);
        bool result = query.exec(
            QString(
                "CREATE TYPE task_state as enum "
                "("
                "'NotStarted', "  // Не начата
                "'Work', "        // В работе
                "'Suspended', "   // Приостановлена
                "'Completed'"     // Завершена
                ")\r\n"
            )
        );

        if (!result) {
            throw DBException(query.lastError(), __FILE__, __LINE__);
        }
    }

    // =========================================================================
    // Инициализация БД (создание таблиц)
    static void initDataBase_(QSqlDatabase& db, const QString& dbname)
    {
        qDebug() << "Init db...";

        // Подключаемся к базе данных
        db.setDatabaseName(dbname);
        DBOpener opener(&db);

        // Создаем таблицу сотрудников
        initTableEmployees_(db);

        // Создаем тип состояние задачи
        initEnumTaskState_(db);

        // Создаем таблицу задач
        initTableTasks_(db);

        if (!db.commit()) {
            throw DBException(db.lastError(), __FILE__, __LINE__);
        }
    }
}

// =============================================================================
void DBPostgresQuery::createNewDB(QSqlDatabase& db,
                                  const QString& dbname,
                                  const QString& owner)
{
    DBOpener opener(&db);

    QSqlQuery query(db);
    bool result = query.exec(
        QString(
            "CREATE DATABASE %1\r\n"
            "\tWITH\r\n"
            "\tOWNER = %2\r\n"
            "\tENCODING = 'UTF8'\r\n"
            "\tLC_COLLATE = 'Russian_Russia.1251'\r\n"
            "\tLC_CTYPE = 'Russian_Russia.1251'\r\n"
            "\tLOCALE_PROVIDER = 'libc'\r\n"
            "\tTABLESPACE = pg_default\r\n"
            "\tCONNECTION LIMIT = -1\r\n"
            "\tIS_TEMPLATE = False;"
        )
        .arg(dbname)
        .arg(owner)
    );

    if (!result) {
        throw DBException(query.lastError(), __FILE__, __LINE__);
    }

    if (!db.commit()) {
        throw DBException(db.lastError(), __FILE__, __LINE__);
    }

    db.close();

    // Инициализация этой БД
    initDataBase_(db, dbname);
}

// =============================================================================
void DBPostgresQuery::checkExistDb(QSqlDatabase& db,
                                   const QString& dbname)
{
    // Пробуем подключиться к БД,
    // если не получится, будет вызвано исключение
    db.setDatabaseName(dbname);
    DBOpener opener(&db);
}

// =============================================================================
void DBPostgresQuery::createNewEmployee(QSqlDatabase& db,
                                        const QString& dbname,
                                        const Employee& employee)
{
    db.setDatabaseName(dbname);
    DBOpener opener(&db);

    QSqlQuery query(db);
    bool result = query.exec(
        QString(
            "INSERT INTO employees ("
            "lastname, "
            "firstname, "
            "patronymic, "
            "position, "
            "email, "
            "phone)\r\n"
            "VALUES ('%1', '%2', '%3', '%4', '%5', '%6');\r\n"
        )
        .arg(employee.lastName())
        .arg(employee.firstName())
        .arg(employee.patronymic())
        .arg(employee.position())
        .arg(employee.email())
        .arg(employee.phone())
    );

    if (!result) {
        throw DBException(query.lastError(), __FILE__, __LINE__);
    }

    if (!db.commit()) {
        throw DBException(db.lastError(), __FILE__, __LINE__);
    }
}

// =============================================================================
void DBPostgresQuery::deleteEmployee(QSqlDatabase& db,
                                     const QString& dbname,
                                     qint32 id)
{
    db.setDatabaseName(dbname);
    DBOpener opener(&db);

    QSqlQuery query(db);
    bool result = query.exec(
        QString(
            "DELETE FROM employees\r\n"
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
QList<Employee> DBPostgresQuery::getEmployees(QSqlDatabase& db,
                                              const QString& dbname)
{
    db.setDatabaseName(dbname);
    DBOpener opener(&db);

    QSqlQuery query(db);
    bool result = query.exec(
        QString(
            "SELECT * FROM employees;\r\n"
        )
    );

    if (!result) {
        throw DBException(query.lastError(), __FILE__, __LINE__);
    }

    QList<Employee> listEmployees;
    while (query.next())
    {
        Employee employee;
        employee.setId(query.value(0).toUInt());
        employee.setLastName(query.value(1).toString());
        employee.setFirstName(query.value(2).toString());
        employee.setPatronymic(query.value(3).toString());
        employee.setPosition(query.value(4).toString());
        employee.setEmail(query.value(5).toString());
        employee.setPhone(query.value(6).toString());

        listEmployees << employee;
    }

    return listEmployees;
}

// =============================================================================
void DBPostgresQuery::changeEmployee(QSqlDatabase& db,
                                     const QString& dbname,
                                     const Employee& employee)
{
    db.setDatabaseName(dbname);
    DBOpener opener(&db);

    QSqlQuery query(db);
    bool result = query.exec(
        QString(
            "UPDATE employees\r\n"
            "SET lastname = '%1',\r\n"
            "    firstname = '%2',\r\n"
            "    patronymic = '%3',\r\n"
            "    position = '%4',\r\n"
            "    email = '%5',\r\n"
            "    phone = '%6'\r\n"
            "WHERE id = %7;\r\n"
        )
        .arg(employee.lastName())
        .arg(employee.firstName())
        .arg(employee.patronymic())
        .arg(employee.position())
        .arg(employee.email())
        .arg(employee.phone())
        .arg(employee.id())
    );

    if (!result) {
        throw DBException(query.lastError(), __FILE__, __LINE__);
    }

    if (!db.commit()) {
        throw DBException(db.lastError(), __FILE__, __LINE__);
    }
}

// =============================================================================
void DBPostgresQuery::changeTask(QSqlDatabase& db,
                                 const QString& dbname,
                                 const Task& task)
{
    db.setDatabaseName(dbname);
    DBOpener opener(&db);

    QSqlQuery query(db);
    bool result = query.exec(
        QString(
            "UPDATE tasks\r\n"
            "SET name = '%1',\r\n"
            "    state = '%2',\r\n"
            "    executor = %3,\r\n"
            "    start = '%4',\r\n"
            "    duration = %5,\r\n"
            "    parent = %6,\r\n"
            "    description = '%7'\r\n"
            "WHERE id = %8;\r\n"
        )
        .arg(task.name())
        .arg(task.stateString())
        .arg(task.executorId())
        .arg(task.startString())
        .arg(task.duration())
        .arg(task.parentId())
        .arg(task.description())
        .arg(task.id())
    );

    if (!result) {
        throw DBException(query.lastError(), __FILE__, __LINE__);
    }

    if (!db.commit()) {
        throw DBException(db.lastError(), __FILE__, __LINE__);
    }
}

// =============================================================================
