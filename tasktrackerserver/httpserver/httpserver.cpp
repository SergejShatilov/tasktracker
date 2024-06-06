
#include "httpserver.h"

// =============================================================================
HttpServer::HttpServer(QObject *parent) :
    QTcpServer(parent)
{
    threadPool = new QThreadPool(this);
}

// =============================================================================
void HttpServer::route(const HttpRequest::Method method,
                       const QString& endPoint,
                       RouteHandler handler)
{
    m_routingMap.insert(QPair(method, endPoint), handler);
}

// =============================================================================
void HttpServer::incomingConnection(qintptr handle)
{
    HttpSocketHandler* runnable = new HttpSocketHandler(handle, m_routingMap);
    threadPool->start(runnable);
}

// =============================================================================
