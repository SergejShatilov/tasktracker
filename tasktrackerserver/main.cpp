
#include <QCoreApplication>

#include "requesthandler.h"
#include "httpserver/httpserver.h"
#include "db/dbpostgres/dbmanagerpostgres.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HttpServer httpServer;

    DBManagerPostgres db("127.0.0.1", 5432);

    RequestHandler(httpServer, &db);

    if (httpServer.listen(QHostAddress::Any, 80)) {
        qDebug() << "Listering...";
    }
    else {
        qDebug() << "Error while starting:" << httpServer.errorString();
    }

    return a.exec();
}
