
#pragma once

#include <QString>
#include <QHash>

class HttpRequest
{
public:
    enum class Method
    {
        Get,
        Post,
        Put,
        Delete
    };

public:
    explicit HttpRequest(const QByteArray& data);

    Method method() const;
    const QString& methodString() const;
    const QString& uri() const;
    const QString& httpVersion() const;
    const QHash<QString, QString>& headers() const;
    const QByteArray& content() const;

    void debug() const;

    static QString uriToEndPoint(const QString& uri);
    static QString findValueInUri(const QString& uri, const QString& key);

private:
    void startLineParse(const QString& startLine);
    void headersParse(const QStringList& headers);
    void checkContent();

private:
    Method                  m_method;
    QString                 m_methodString;
    QString                 m_uri;
    QString                 m_httpVersion;
    QHash<QString, QString> m_headers;
    QByteArray              m_content;
};
