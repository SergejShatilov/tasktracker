
#include "response.h"

#include <QMap>

Response::Response(const Status status)
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

void Response::setData(const QString& data) {
    m_data = data;
}

void Response::addHeader(const QString& key, const QString& value) {
    m_headers.insert(key, value);
}

const QByteArray Response::toByteArray() const
{
    QString response = "HTTP/1.1 " + m_status + "\r\n";
    for (auto it = m_headers.cbegin(); it != m_headers.cend(); ++it) {
        response += (it.key() + ": " + it.value() + "\r\n");
    }
    response += "\r\n";
    response += m_data;

    return response.toLocal8Bit();
}
