
#include "requesthandler.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#include "db/dbexception.h"
#include "employee.h"

// =============================================================================
RequestHandler::RequestHandler(HttpServer& httpServer,
                               DbManager* db,
                               QObject* parent) :
    QObject(parent),
    m_httpServer(httpServer),
    m_db(db)
{
    // Регистрируем все группы обработчиков запросов
    registerHandlersForDb();
    registerHandlersForEmployees();
    registerHandlersForTasks();
}

// =============================================================================
void RequestHandler::registerHandlersForDb()
{
    m_httpServer.route(
        HttpRequest::Method::Post,
        "/",
        [this](const auto& content, const auto& args) {
            return handlerCreateDb(content, args);
        }
    );

    m_httpServer.route(
        HttpRequest::Method::Get,
        "/{dbname}",
        [this](const auto& content, const auto& args) {
            return handlerCheckExistDb(content, args);
        }
    );
}

// =============================================================================
void RequestHandler::registerHandlersForEmployees()
{
    m_httpServer.route(
        HttpRequest::Method::Post,
        "/{dbname}/employees",
        [this](const auto& content, const auto& args) {
            return handlerCreateEmployee(content, args);
        }
    );

    m_httpServer.route(
        HttpRequest::Method::Delete,
        "/{dbname}/employees/{id}",
        [this](const auto& content, const auto& args) {
            return handlerDeleteEmployee(content, args);
        }
    );

    m_httpServer.route(
        HttpRequest::Method::Put,
        "/{dbname}/employees/{id}",
        [this](const auto& content, const auto& args) {
            return handlerChangeEmployee(content, args);
        }
    );

    m_httpServer.route(
        HttpRequest::Method::Get,
        "/{dbname}/employees",
        [this](const auto& content, const auto& args) {
            return handlerGetEmployees(content, args);
        }
    );
}

// =============================================================================
void RequestHandler::registerHandlersForTasks()
{
    m_httpServer.route(
        HttpRequest::Method::Post,
        "/{dbname}/tasks",
        [this](const auto& content, const auto& args) {
            return handlerCreateTask(content, args);
        }
    );

    m_httpServer.route(
        HttpRequest::Method::Delete,
        "/{dbname}/tasks/{id}",
        [this](const auto& content, const auto& args) {
            return handlerDeleteTask(content, args);
        }
    );

    m_httpServer.route(
        HttpRequest::Method::Put,
        "/{dbname}/tasks/{id}",
        [this](const auto& content, const auto& args) {
            return handlerChangeTask(content, args);
        }
    );

    m_httpServer.route(
        HttpRequest::Method::Get,
        "/{dbname}/tasks",
        [this](const auto& content, const auto& args) {
            return handlerGetTasks(content, args);
        }
    );
}

// =============================================================================
HttpResponse RequestHandler::handlerCreateDb(
    const QByteArray& content,
    const RoutingArgs& args)
{
    Q_UNUSED(args);

    QJsonDocument jdoc(QJsonDocument::fromJson(content));
    QJsonObject jobj = jdoc.object();
    const QString dbName = jobj["dbName"].toString();

    try {
        m_db->createDb(dbName);
    }
    catch (const DBException& ex)
    {
        qDebug() << ex;
        return HttpResponse(
            HttpResponse::Status::BadRequest,
            ex.error().text().toLocal8Bit()
        );
    }

    return HttpResponse(HttpResponse::Status::Created);
}

// =============================================================================
HttpResponse RequestHandler::handlerCheckExistDb(
    const QByteArray& content,
    const RoutingArgs& args)
{
    Q_UNUSED(content);

    const QString dbName = args["{dbname}"].toString();

    try {
        m_db->checkExistDb(dbName);
    }
    catch (const DBException& ex)
    {
        qDebug() << ex;
        return HttpResponse(
            HttpResponse::Status::BadRequest,
            ex.error().text().toLocal8Bit()
        );
    }

    return HttpResponse(HttpResponse::Status::OK);
}

// =============================================================================
HttpResponse RequestHandler::handlerCreateEmployee(
    const QByteArray& content,
    const RoutingArgs& args)
{
    const QString dbName = args["{dbname}"].toString();

    auto employee = QSharedPointer<Employee>(
        Employee::createFromJson(content, this)
    );

    try {
        m_db->createEmployee(dbName, employee);
    }
    catch (const DBException& ex)
    {
        qDebug() << ex;
        return HttpResponse(
            HttpResponse::Status::BadRequest,
            ex.error().text().toLocal8Bit()
        );
    }

    return HttpResponse(
        HttpResponse::Status::Created,
        employee->toJson()
    );
}

// =============================================================================
HttpResponse RequestHandler::handlerDeleteEmployee(
    const QByteArray& content, const RoutingArgs& args)
{
    Q_UNUSED(content);

    const QString dbName = args["{dbname}"].toString();
    const qint32  id     = args["{id}"].toInt();

    try {
        m_db->deleteEmployee(dbName, id);
    }
    catch (const DBException& ex)
    {
        qDebug() << ex;
        return HttpResponse(
            HttpResponse::Status::BadRequest,
            ex.error().text().toLocal8Bit()
        );
    }

    return HttpResponse(HttpResponse::Status::OK);
}

// =============================================================================
HttpResponse RequestHandler::handlerChangeEmployee(
    const QByteArray& content,
    const RoutingArgs& args)
{
    const QString dbName = args["{dbname}"].toString();

    const auto employee = QSharedPointer<Employee>(
        Employee::createFromJson(content, this)
    );

    try {
        m_db->changeEmployee(dbName, employee);
    }
    catch (const DBException& ex)
    {
        qDebug() << ex;
        return HttpResponse(
            HttpResponse::Status::BadRequest,
            ex.error().text().toLocal8Bit()
        );
    }

    return HttpResponse(HttpResponse::Status::OK);
}

// =============================================================================
HttpResponse RequestHandler::handlerGetEmployees(
    const QByteArray& content,
    const RoutingArgs& args)
{
    Q_UNUSED(content);

    const QString dbName = args["{dbname}"].toString();

    QList<QSharedPointer<Employee>> listEmployees;

    try {
        listEmployees = m_db->getEmployees(dbName);
    }
    catch (const DBException& ex)
    {
        qDebug() << ex;
        return HttpResponse(
            HttpResponse::Status::BadRequest,
            ex.error().text().toLocal8Bit()
        );
    }

    return HttpResponse (
        HttpResponse::Status::OK,
        Employee::listToJson(listEmployees)
    );
}

// =============================================================================
HttpResponse RequestHandler::handlerCreateTask(
    const QByteArray& content,
    const RoutingArgs& args)
{
    const QString dbName = args["{dbname}"].toString();

    auto task = QSharedPointer<Task>(Task::createFromJson(content, this));

    try {
        m_db->createTask(dbName, task);
    }
    catch (const DBException& ex)
    {
        qDebug() << ex;
        return HttpResponse(
            HttpResponse::Status::BadRequest,
            ex.error().text().toLocal8Bit()
        );
    }

    return HttpResponse(
        HttpResponse::Status::Created,
        task->toJson()
    );
}

// =============================================================================
HttpResponse RequestHandler::handlerDeleteTask(
    const QByteArray& content,
    const RoutingArgs& args)
{
    Q_UNUSED(content);

    const QString dbName = args["{dbname}"].toString();
    const qint32  id     = args["{id}"].toInt();

    try {
        m_db->deleteTask(dbName, id);
    }
    catch (const DBException& ex)
    {
        qDebug() << ex;
        return HttpResponse(
            HttpResponse::Status::BadRequest,
            ex.error().text().toLocal8Bit()
        );
    }

    return HttpResponse(HttpResponse::Status::OK);
}

// =============================================================================
HttpResponse RequestHandler::handlerChangeTask(
    const QByteArray& content,
    const RoutingArgs& args)
{
    const QString dbName = args["{dbname}"].toString();

    const auto task = QSharedPointer<Task>(
        Task::createFromJson(content, this)
    );

    try {
        m_db->changeTask(dbName, task);
    }
    catch (const DBException& ex)
    {
        qDebug() << ex;
        return HttpResponse(
            HttpResponse::Status::BadRequest,
            ex.error().text().toLocal8Bit()
        );
    }

    return HttpResponse(HttpResponse::Status::OK);
}

// =============================================================================
HttpResponse RequestHandler::handlerGetTasks(
    const QByteArray& content,
    const RoutingArgs& args)
{
    Q_UNUSED(content);

    const QString dbName = args["{dbname}"].toString();

    QList<QSharedPointer<Task>> listTasks;

    try {
        listTasks = m_db->getTasks(dbName);
    }
    catch (const DBException& ex)
    {
        qDebug() << ex;
        return HttpResponse(
            HttpResponse::Status::BadRequest,
            ex.error().text().toLocal8Bit()
        );
    }

    return HttpResponse (
        HttpResponse::Status::OK,
        Task::listToJson(listTasks)
    );
}

// =============================================================================
