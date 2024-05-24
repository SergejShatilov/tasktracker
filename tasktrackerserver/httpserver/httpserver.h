
#pragma once

#include <QTcpServer>
#include <QThreadPool>
#include <functional>

#include "sockethandler.h"

class HttpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit HttpServer(QObject* parent = nullptr);

    void route(const Request::Method method,
               const QString& endPoint,
               RouteHandler handler);

private:
    QThreadPool* threadPool;

    void incomingConnection(qintptr handle) override;

private:
    RoutingMap m_routingMap;
};
