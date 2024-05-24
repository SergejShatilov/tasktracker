
#pragma once

#include <QRunnable>
#include <QTcpSocket>
#include <QDebug>
#include <QTime>

#include "request.h"
#include "response.h"

using RouteHandler = std::function<Response(const Request&)>;
using RoutingMap = QMap<QPair<Request::Method, QString>, RouteHandler>;

class SocketHandler : public QRunnable
{
public:
    SocketHandler(int handle, const RoutingMap& routingMap);

private:
    void run() override;
    Response requestProcess(const Request& request);

private:
    const int         m_handle;
    const RoutingMap& m_routingMap;
};
