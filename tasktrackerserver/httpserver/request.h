
#pragma once

#include <QString>
#include <QHash>

class Request
{
public:
    enum class Method
    {
        Invalid,
        Get,
        Post,
        Put,
        Delete,
        Patch
    };

public:
    Request(const QString& request);

    Method method() const;
    const QString& methodString() const;
    const QString& uri() const;
    const QString& httpVersion() const;

    const QHash<QString, QString>& headers() const;

    const QString& data() const;

    static const QString uriToEndPoint(const QString& uri);

private:
    void startLineParse(const QString& startLine);
    void headersParse(const QStringList& headers);

private:
    Method  m_method;
    QString m_methodString;
    QString m_uri;
    QString m_httpVersion;

    QHash<QString, QString> m_headers;

    QString m_data;

};
