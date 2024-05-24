
#include "httpserver.h"

HttpServer::HttpServer(QObject *parent) :
    QTcpServer(parent)
{
    if (listen(QHostAddress::Any, 80)) {
        qDebug() << "Listering...";
    }
    else {
        qDebug() << "Error while starting:" << errorString();
    }

    threadPool = new QThreadPool(this);
}

void HttpServer::route(const Request::Method method,
                       const QString& endPoint,
                       RouteHandler handler)
{
    m_routingMap.insert(QPair(method, endPoint), handler);
}

void HttpServer::incomingConnection(qintptr handle)
{
    SocketHandler* runnable = new SocketHandler(handle, m_routingMap);
    threadPool->start(runnable);
}
