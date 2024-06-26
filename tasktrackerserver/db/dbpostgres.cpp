
#include "dbpostgres.h"

#include <QSqlQuery>
#include "dbopener.h"

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
                "\tid SERIAL PRIMARY KEY,\r\n"                  // ID задачи
                "\tname CHARACTER VARYING(60),\r\n"             // Наименование задачи
                "\tstate task_state,\r\n"                       // Состояние задачи
                "\texecutor INTEGER,\r\n"                       // ID исполнителя
                "\tdeadline DATE,\r\n"                          // Дата начала
                "\tparent INTEGER,\r\n"                         // ID родительской задачи
                "\tdescription TEXT\r\n"                        // Описание задачи
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
    static void initDataBase_(QSqlDatabase& db, const QString& dbName)
    {
        qDebug() << "Init db...";

        // Подключаемся к базе данных
        db.setDatabaseName(dbName);
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
DbPostgres::DbPostgres(const QString& hostName, int port) :
    m_hostName(hostName),
    m_port(port)
{

}

// =============================================================================
QString DbPostgres::dbType() const {
    return QString("QPSQL");
}

// =============================================================================
void DbPostgres::createDb(const QString& dbName)
{
    QSqlDatabase db = addDatabase();

    DBOpener opener(&db);

    QSqlQuery query(db);
    bool result = query.exec(
        QString(
            "CREATE DATABASE %1\r\n"
            "\tWITH\r\n"
            "\tOWNER = postgres\r\n"
            "\tENCODING = 'UTF8'\r\n"
            "\tLC_COLLATE = 'ru_RU.UTF-8'\r\n"
            "\tLC_CTYPE = 'ru_RU.UTF-8'\r\n"
            "\tTEMPLATE template0;\r\n"
        )
        .arg(dbName)
    );

    if (!result) {
        throw DBException(query.lastError(), __FILE__, __LINE__);
    }

    if (!db.commit()) {
        throw DBException(db.lastError(), __FILE__, __LINE__);
    }

    db.close();

    // Инициализация этой БД
    initDataBase_(db, dbName);
}

// =============================================================================
void DbPostgres::checkExistDb(const QString& dbName)
{
    QSqlDatabase db = addDatabase();

    // Пробуем подключиться к БД,
    // если не получится, будет вызвано исключение
    db.setDatabaseName(dbName);
    DBOpener opener(&db);
}

// =============================================================================
void DbPostgres::createEmployee(
    const QString& dbName,
    QSharedPointer<Employee> employee)
{
    QSqlDatabase db = addDatabase();

    db.setDatabaseName(dbName);
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
            "VALUES ('%1', '%2', '%3', '%4', '%5', '%6') "
            "RETURNING id;\r\n"
        )
        .arg(employee->lastName())
        .arg(employee->firstName())
        .arg(employee->patronymic())
        .arg(employee->position())
        .arg(employee->email())
        .arg(employee->phone())
    );

    if (!result) {
        throw DBException(query.lastError(), __FILE__, __LINE__);
    }

    if (!db.commit()) {
        throw DBException(db.lastError(), __FILE__, __LINE__);
    }

    employee->setId(query.lastInsertId().toInt());
}

// =============================================================================
void DbPostgres::deleteEmployee(const QString& dbName, qint32 id)
{
    QSqlDatabase db = addDatabase();

    db.setDatabaseName(dbName);
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
void DbPostgres::changeEmployee(
    const QString& dbName,
    const QSharedPointer<Employee> employee)
{
    QSqlDatabase db = addDatabase();

    db.setDatabaseName(dbName);
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
        .arg(employee->lastName())
        .arg(employee->firstName())
        .arg(employee->patronymic())
        .arg(employee->position())
        .arg(employee->email())
        .arg(employee->phone())
        .arg(employee->id())
    );

    if (!result) {
        throw DBException(query.lastError(), __FILE__, __LINE__);
    }

    if (!db.commit()) {
        throw DBException(db.lastError(), __FILE__, __LINE__);
    }
}

// =============================================================================
QList<QSharedPointer<Employee>> DbPostgres::getEmployees(const QString& dbName)
{
    QSqlDatabase db = addDatabase();

    db.setDatabaseName(dbName);
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

    QList<QSharedPointer<Employee>> listEmployees;
    while (query.next())
    {
        auto employee = QSharedPointer<Employee>(new Employee());

        employee->setId(query.value(0).toUInt());
        employee->setLastName(query.value(1).toString());
        employee->setFirstName(query.value(2).toString());
        employee->setPatronymic(query.value(3).toString());
        employee->setPosition(query.value(4).toString());
        employee->setEmail(query.value(5).toString());
        employee->setPhone(query.value(6).toString());

        listEmployees << employee;
    }

    return listEmployees;
}

// =============================================================================
void DbPostgres::createTask(const QString& dbName, QSharedPointer<Task> task)
{
    QSqlDatabase db = addDatabase();

    db.setDatabaseName(dbName);
    DBOpener opener(&db);

    QSqlQuery query(db);
    bool result = query.exec(
        QString(
            "INSERT INTO tasks ("
            "name, "
            "state, "
            "executor, "
            "deadline, "
            "parent, "
            "description)\r\n"
            "VALUES ('%1', '%2', %3, '%4', %5, '%6') "
            "RETURNING id;\r\n"
        )
        .arg(task->name())
        .arg(task->stateString())
        .arg(task->executorId())
        .arg(task->deadlineString())
        .arg(task->parentId())
        .arg(task->description())
    );

    if (!result) {
        throw DBException(query.lastError(), __FILE__, __LINE__);
    }

    if (!db.commit()) {
        throw DBException(db.lastError(), __FILE__, __LINE__);
    }

    task->setId(query.lastInsertId().toInt());
}

// =============================================================================
void DbPostgres::deleteTask(const QString& dbName, qint32 id)
{
    QSqlDatabase db = addDatabase();

    db.setDatabaseName(dbName);
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
void DbPostgres::changeTask(const QString& dbName,
                            const QSharedPointer<Task> task)
{
    QSqlDatabase db = addDatabase();

    db.setDatabaseName(dbName);
    DBOpener opener(&db);

    QSqlQuery query(db);
    bool result = query.exec(
        QString(
            "UPDATE tasks\r\n"
            "SET name = '%1',\r\n"
            "    state = '%2',\r\n"
            "    executor = %3,\r\n"
            "    deadline = '%4',\r\n"
            "    parent = %5,\r\n"
            "    description = '%6'\r\n"
            "WHERE id = %7;\r\n"
        )
        .arg(task->name())
        .arg(task->stateString())
        .arg(task->executorId())
        .arg(task->deadlineString())
        .arg(task->parentId())
        .arg(task->description())
        .arg(task->id())
    );

    if (!result) {
        throw DBException(query.lastError(), __FILE__, __LINE__);
    }

    if (!db.commit()) {
        throw DBException(db.lastError(), __FILE__, __LINE__);
    }
}

// =============================================================================
QList<QSharedPointer<Task>> DbPostgres::getTasks(const QString& dbName)
{
    QSqlDatabase db = addDatabase();

    db.setDatabaseName(dbName);
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

    QList<QSharedPointer<Task>> listTasks;
    while (query.next())
    {
        auto task = QSharedPointer<Task>(new Task());

        task->setId(query.value(0).toUInt());
        task->setName(query.value(1).toString());
        task->setStateString(query.value(2).toString());
        task->setExecutorId(query.value(3).toUInt());
        task->setDeadlineString(query.value(4).toString());
        task->setParentId(query.value(5).toUInt());
        task->setDescription(query.value(6).toString());
        listTasks << task;
    }

    return listTasks;
}

// =============================================================================
QSqlDatabase
DbPostgres::addDatabase() const
{
    QSqlDatabase db = QSqlDatabase::addDatabase(dbType());
    db.setHostName(m_hostName);
    db.setPort(m_port);
    db.setUserName("postgres");
    db.setPassword("123");

    return db;
}

// =============================================================================
