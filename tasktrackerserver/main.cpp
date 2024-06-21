
#include <QCoreApplication>

#include "requesthandler.h"
#include "httpserver/httpserver.h"
#include "db/dbpostgres.h"

#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    HttpServer httpServer;

    DbPostgres db("127.0.0.1", 5432);

    RequestHandler requestHandler(httpServer, &db);

    if (httpServer.listen(QHostAddress::Any, 80)) {
        qDebug() << "Listering...";
    }
    else {
        qDebug() << "Error while starting:" << httpServer.errorString();
    }

    return a.exec();
}
