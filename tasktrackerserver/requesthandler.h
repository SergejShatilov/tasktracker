
#pragma once

#include "httpserver/httpserver.h"
#include "db/dbmanager.h"

class RequestHandler : public QObject
{
    Q_OBJECT

public:
    explicit RequestHandler(HttpServer& httpServer,
                            DbManager* db = nullptr,
                            QObject* parent = nullptr);

private:
    void registerHandlersForDb();
    void registerHandlersForEmployees();
    void registerHandlersForTasks();

private:
    HttpResponse handlerCreateDb(const QByteArray& content,
                                 const RoutingArgs& args);
    HttpResponse handlerCheckExistDb(const QByteArray& content,
                                     const RoutingArgs& args);

    HttpResponse handlerCreateEmployee(const QByteArray& content,
                                       const RoutingArgs& args);
    HttpResponse handlerDeleteEmployee(const QByteArray& content,
                                       const RoutingArgs& args);
    HttpResponse handlerChangeEmployee(const QByteArray& content,
                                       const RoutingArgs& args);
    HttpResponse handlerGetEmployees(const QByteArray& content,
                                     const RoutingArgs& args);

    HttpResponse handlerCreateTask(const QByteArray& content,
                                   const RoutingArgs& args);
    HttpResponse handlerDeleteTask(const QByteArray& content,
                                   const RoutingArgs& args);
    HttpResponse handlerChangeTask(const QByteArray& content,
                                   const RoutingArgs& args);
    HttpResponse handlerGetTasks(const QByteArray& content,
                                 const RoutingArgs& args);

private:
    HttpServer& m_httpServer;
    DbManager*  m_db;
};
