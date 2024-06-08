
#pragma once

#include "httpserver/httpserver.h"
#include "db/dbmanager.h"

class RequestHandler
{
public:
    explicit RequestHandler(HttpServer& httpServer,
                            DBManager* db = nullptr);

private:
    HttpResponse handlerCreateNewDB(const HttpRequest& request);
    HttpResponse handlerCheckExistDb(const HttpRequest& request);

    HttpResponse handlerCreateNewEmployee(const HttpRequest& request);
    HttpResponse handlerDeleteEmployee(const HttpRequest& request);
    HttpResponse handlerGetEmployees(const HttpRequest& request);
    HttpResponse handlerChangeEmployee(const HttpRequest& request);

    HttpResponse handlerCreateNewTask(const HttpRequest& request);
    HttpResponse handlerGetTasks(const HttpRequest& request);
    HttpResponse handlerDeleteTask(const HttpRequest& request);
    HttpResponse handlerChangeTask(const HttpRequest& request);

private:
    DBManager* m_db;
};
