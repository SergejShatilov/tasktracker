
#pragma once

#include <QString>
#include <QHash>
#include <QDebug>

class HttpRequest
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
    explicit HttpRequest(const QByteArray& request);

    Method method() const;
    const QString& methodString() const;
    const QString& uri() const;
    const QString& httpVersion() const;

    const QHash<QString, QString>& headers() const;

    const QString& userName() const;
    const QString& password() const;
    QString dbname() const;
    qint32 id() const;

    const QByteArray& data() const;

    static QString uriToEndPoint(const QString& uri);
    static QString findValueInUri(const QString& uri, const QString& key);

private:
    void startLineParse(const QString& startLine);
    void headersParse(const QStringList& headers);

private:
    Method  m_method;
    QString m_methodString;
    QString m_uri;
    QString m_httpVersion;
    QString m_userName;
    QString m_password;

    QHash<QString, QString> m_headers;

    QByteArray m_data;
};

inline QDebug operator<<(QDebug d, const HttpRequest& ex)
{
    d << "Request:\r\n";
    d << "\tMethod:" << ex.methodString() << "\r\n";
    d << "\tUri:" << ex.uri() << "\r\n";
    d << "\tHttp Version:" << ex.httpVersion() << "\r\n";
    d << "\tHeaders:" << "\r\n";
    const auto& headers = ex.headers();
    for (auto it = headers.cbegin(); it != headers.cend(); ++it) {
        d << "\t" << it.key() << ":" << it.value() << "\r\n";
    }
    d << "\tData:" << ex.data() << "\r\n";

    return d;
}
