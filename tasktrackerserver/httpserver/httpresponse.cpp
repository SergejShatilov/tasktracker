
#include "httpresponse.h"

#include <QMap>

// =============================================================================
HttpResponse::HttpResponse(const Status status)
{
    static QMap<Status, QString> const tableStatuses {
        {Status::OK,         "200 OK"},
        {Status::Created,    "201 Created"},
        {Status::BadRequest, "400 Bad Request"},
        {Status::NotFound,   "404 Not Found"}
    };

    const auto it = tableStatuses.find(status);
    if (it == tableStatuses.end())
        throw std::runtime_error("internal error, responce status is invalid");

    m_status = it.value();
}

// =============================================================================
HttpResponse::HttpResponse(const Status status, const QByteArray& data) :
    HttpResponse(status)
{
    m_data = data;
}

// =============================================================================
void HttpResponse::setData(const QByteArray& data) {
    m_data = data;
}

// =============================================================================
void HttpResponse::addHeader(const QString& key, const QString& value) {
    m_headers.insert(key, value);
}

// =============================================================================
QByteArray HttpResponse::toByteArray() const
{
    QString response = "HTTP/1.1 " + m_status + "\r\n";
    for (auto it = m_headers.cbegin(); it != m_headers.cend(); ++it) {
        response += (it.key() + ": " + it.value() + "\r\n");
    }
    response += "\r\n";
    response += m_data;

    return response.toLocal8Bit();
}

// =============================================================================
