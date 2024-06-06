
#pragma once

#include <QRunnable>
#include <QTcpSocket>
#include <QDebug>
#include <QTime>

#include "httprequest.h"
#include "httpresponse.h"

using RouteHandler = std::function<HttpResponse(const HttpRequest&)>;
using RoutingMap = QMap<QPair<HttpRequest::Method, QString>, RouteHandler>;

class HttpSocketHandler : public QRunnable
{
public:
    explicit HttpSocketHandler(int handle, const RoutingMap& routingMap);

private:
    void run() override;
    HttpResponse requestProcess(const HttpRequest& request);

private:
    const int         m_handle;
    const RoutingMap& m_routingMap;
};
