
#pragma once

#include <QString>
#include <QHash>

class HttpResponse
{
public:
    enum class Status
    {
        OK          = 200,
        Created     = 201,
        BadRequest  = 400,
        NotFound    = 404
    };

public:
    explicit HttpResponse(const Status status);
    explicit HttpResponse(const Status status, const QByteArray& data);

    void setData(const QByteArray& data);
    void addHeader(const QString& key, const QString& value);

    QByteArray toByteArray() const;

private:
    QString m_status;
    QByteArray m_data;
    QHash<QString, QString> m_headers;
};
