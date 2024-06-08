
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
RequestHandler::RequestHandler(HttpServer& httpServer, DBManager* db) :
    m_db(db)
{
    // Регистрируем обработчик создания базы данных сотрудников и задач
    httpServer.route(
        HttpRequest::Method::Post,
        "/",
        [this](const HttpRequest& request) {
            return handlerCreateNewDB(request);
        }
    );

    // Регистрируем обработчик проверки существования БД
    httpServer.route(
        HttpRequest::Method::Get,
        "/dbname-{dbname}/",
        [this](const HttpRequest& request) {
            return handlerCheckExistDb(request);
        }
    );

    // Регистрируем обработчик создания нового сотрудника
    httpServer.route(
        HttpRequest::Method::Post,
        "/dbname-{dbname}/employees/",
        [this](const HttpRequest& request) {
            return handlerCreateNewEmployee(request);
        }
    );

    // Регистрируем обработчик удаления сотрудника
    httpServer.route(
        HttpRequest::Method::Delete,
        "/dbname-{dbname}/employees/id-{id}/",
        [this](const HttpRequest& request) {
            return handlerDeleteEmployee(request);
        }
    );

    // Регистрируем обработчик запроса сотрудников
    httpServer.route(
        HttpRequest::Method::Get,
        "/dbname-{dbname}/employees/",
        [this](const HttpRequest& request) {
            return handlerGetEmployees(request);
        }
    );

    // Регистрируем обработчик изменения данных о сотруднике
    httpServer.route(
        HttpRequest::Method::Put,
        "/dbname-{dbname}/employees/id-{id}/",
        [this](const HttpRequest& request) {
            return handlerChangeEmployee(request);
        }
    );

    // Регистрируем обработчик создания новой задачи
    httpServer.route(
        HttpRequest::Method::Post,
        "/dbname-{dbname}/tasks/",
        [this](const HttpRequest& request) {
            return handlerCreateNewTask(request);
        }
    );

    // Регистрируем обработчик запроса задач
    httpServer.route(
        HttpRequest::Method::Get,
        "/dbname-{dbname}/tasks/",
        [this](const HttpRequest& request) {
            return handlerGetTasks(request);
        }
    );

    // Регистрируем обработчик удаления задачи
    httpServer.route(
        HttpRequest::Method::Delete,
        "/dbname-{dbname}/tasks/id-{id}/",
        [this](const HttpRequest& request) {
            return handlerDeleteTask(request);
        }
    );

    // Регистрируем обработчик изменения данных о задаче
    httpServer.route(
        HttpRequest::Method::Put,
        "/dbname-{dbname}/tasks/id-{id}/",
        [this](const HttpRequest& request) {
            return handlerChangeTask(request);
        }
    );
}

// =============================================================================
HttpResponse RequestHandler::handlerCreateNewDB(const HttpRequest &request)
{
    qDebug() << "Create new db...";

    // Получаем имя создаваемой базы данных
    QJsonDocument jdoc(QJsonDocument::fromJson(request.data()));
    QJsonObject jobj = jdoc.object();
    const QString dbname = jobj["dbname"].toString();

    // Объект для авторизации
    DBAuthorization authorization(request.userName(), request.password());

    try {
        m_db->createNewDB(authorization, dbname, authorization.userName());
    } catch (const DBException& ex) {
        qDebug() << ex;
        return HttpResponse(HttpResponse::Status::BadRequest,
                            ex.error().text().toLocal8Bit());
    }

    return HttpResponse(HttpResponse::Status::Created);
}

// =============================================================================
HttpResponse RequestHandler::handlerCheckExistDb(const HttpRequest& request)
{
    qDebug() << "Check exist db...";

    // Объект для авторизации
    DBAuthorization authorization(request.userName(), request.password());

    try {
        m_db->checkExistDb(authorization, request.dbname());
    } catch (const DBException& ex) {
        qDebug() << ex;
        return HttpResponse(HttpResponse::Status::BadRequest,
                            ex.error().text().toLocal8Bit());
    }

    return HttpResponse(HttpResponse::Status::OK);
}

// =============================================================================
HttpResponse
RequestHandler::handlerCreateNewEmployee(const HttpRequest &request)
{
    qDebug() << "Create new employee...";

    // Получаем данные сотрудника
    QJsonDocument jdoc(QJsonDocument::fromJson(request.data()));
    Employee employee = Employee::fromJsonObject(jdoc.object());

    // Объект для авторизации
    DBAuthorization authorization(request.userName(), request.password());

    try {
        m_db->createNewEmployee(authorization, request.dbname(), employee);
    } catch (const DBException& ex) {
        qDebug() << ex;
        return HttpResponse(HttpResponse::Status::BadRequest,
                            ex.error().text().toLocal8Bit());
    }

    return HttpResponse(HttpResponse::Status::Created);
}

// =============================================================================
HttpResponse RequestHandler::handlerDeleteEmployee(const HttpRequest& request)
{
    qDebug() << "Delete employees...";

    // Объект для авторизации
    DBAuthorization authorization(request.userName(), request.password());

    try {
        m_db->deleteEmployee(authorization, request.dbname(), request.id());
    } catch (const DBException& ex) {
        qDebug() << ex;
        return HttpResponse(HttpResponse::Status::BadRequest,
                            ex.error().text().toLocal8Bit());
    }

    return HttpResponse(HttpResponse::Status::OK);
}

// =============================================================================
HttpResponse RequestHandler::handlerGetEmployees(const HttpRequest &request)
{
    qDebug() << "Get employees...";

    // Объект для авторизации
    DBAuthorization authorization(request.userName(), request.password());
    QList<Employee> listEmployees;

    try {
        listEmployees = m_db->getEmployees(authorization, request.dbname());
    } catch (const DBException& ex) {
        qDebug() << ex;
        return HttpResponse(HttpResponse::Status::BadRequest,
                            ex.error().text().toLocal8Bit());
    }

    QJsonArray jarray;
    for (const auto& employee : listEmployees) {
        jarray.append(employee.toJsonObject());
    }

    QJsonObject jobj;
    jobj.insert("employees", QJsonValue(jarray));

    QJsonDocument jdoc;
    jdoc.setObject(jobj);

    return HttpResponse
    (
        HttpResponse::Status::OK,
        QJsonDocument(jobj).toJson(QJsonDocument::JsonFormat::Compact)
    );
}

// =============================================================================
HttpResponse RequestHandler::handlerChangeEmployee(const HttpRequest& request)
{
    qDebug() << "Change employee...";

    // Получаем данные сотрудника
    QJsonDocument jdoc(QJsonDocument::fromJson(request.data()));
    Employee employee = Employee::fromJsonObject(jdoc.object());

    // Объект для авторизации
    DBAuthorization authorization(request.userName(), request.password());

    try {
        m_db->changeEmployee(authorization, request.dbname(), employee);
    } catch (const DBException& ex) {
        qDebug() << ex;
        return HttpResponse(HttpResponse::Status::BadRequest,
                            ex.error().text().toLocal8Bit());
    }

    return HttpResponse(HttpResponse::Status::OK);
}

// =============================================================================
HttpResponse
RequestHandler::handlerCreateNewTask(const HttpRequest &request)
{
    qDebug() << "Create new task...";

    // Получаем данные сотрудника
    QJsonDocument jdoc(QJsonDocument::fromJson(request.data()));
    Task task = Task::fromJsonObject(jdoc.object());

    // Объект для авторизации
    DBAuthorization authorization(request.userName(), request.password());

    try {
        m_db->createNewTask(authorization, request.dbname(), task);
    } catch (const DBException& ex) {
        qDebug() << ex;
        return HttpResponse(HttpResponse::Status::BadRequest,
                            ex.error().text().toLocal8Bit());
    }

    return HttpResponse(HttpResponse::Status::Created);
}

// =============================================================================
HttpResponse RequestHandler::handlerGetTasks(const HttpRequest &request)
{
    qDebug() << "Get tasks...";

    DBAuthorization authorization(request.userName(), request.password());
    QList<Task> listTasks;

    try {
        listTasks = m_db->getTasks(authorization, request.dbname());
    } catch (const DBException& ex) {
        qDebug() << ex;
        return HttpResponse(HttpResponse::Status::BadRequest,
                            ex.error().text().toLocal8Bit());
    }

    QJsonArray jarray;
    for (const auto& employee : listTasks) {
        jarray.append(employee.toJsonObject());
    }

    QJsonObject jobj;
    jobj.insert("tasks", QJsonValue(jarray));

    QJsonDocument jdoc;
    jdoc.setObject(jobj);

    return HttpResponse
    (
        HttpResponse::Status::OK,
        QJsonDocument(jobj).toJson(QJsonDocument::JsonFormat::Compact)
    );
}

// =============================================================================
HttpResponse RequestHandler::handlerDeleteTask(const HttpRequest& request)
{
    qDebug() << "Delete task...";

    // Объект для авторизации
    DBAuthorization authorization(request.userName(), request.password());

    try {
        m_db->deleteTask(authorization, request.dbname(), request.id());
    } catch (const DBException& ex) {
        qDebug() << ex;
        return HttpResponse(HttpResponse::Status::BadRequest,
                            ex.error().text().toLocal8Bit());
    }

    return HttpResponse(HttpResponse::Status::OK);
}

// =============================================================================
HttpResponse RequestHandler::handlerChangeTask(const HttpRequest& request)
{
    qDebug() << "Change task...";

    // Получаем данные сотрудника
    QJsonDocument jdoc(QJsonDocument::fromJson(request.data()));
    Task task = Task::fromJsonObject(jdoc.object());

    // Объект для авторизации
    DBAuthorization authorization(request.userName(), request.password());

    try {
        m_db->changeTask(authorization, request.dbname(), task);
    } catch (const DBException& ex) {
        qDebug() << ex;
        return HttpResponse(HttpResponse::Status::BadRequest,
                            ex.error().text().toLocal8Bit());
    }

    return HttpResponse(HttpResponse::Status::OK);
}

// =============================================================================
