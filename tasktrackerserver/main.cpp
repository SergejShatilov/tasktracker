#include <QCoreApplication>

#include "httpserver/httpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HttpServer httpServer;

    httpServer.route(Request::Method::Get, "/data/{id}", [](const Request&) {
        Response response(Response::Status::OK);
        response.setData(QTime::currentTime().toString());
        return response;
    });

    //QSimpleServer server;

    //QMultiThreadServer server;

    //QPoolThreadServer server;

    return a.exec();
}
