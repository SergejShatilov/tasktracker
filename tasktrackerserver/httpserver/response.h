
#pragma once

#include <QString>
#include <QHash>

class Response
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
    Response(const Status status);

    void setData(const QString& data);
    void addHeader(const QString& key, const QString& value);

    const QByteArray toByteArray() const;

private:
    QString m_status;
    QString m_data;
    QHash<QString, QString> m_headers;
};
