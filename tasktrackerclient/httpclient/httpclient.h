
#pragma once

#include <QObject>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>

#define HTTPCLENT_DEFAULT_TIMEOUT_MS    (30000)

class HttpClient : public QObject
{
    Q_OBJECT

public:
    enum class Method
    {
        Get,
        Post,
        Put,
        Delete
    };

public:
    explicit HttpClient(QObject* parent = nullptr);

    void setHostName(const QString& hostName);
    const QString& hostName() const;
    void setPort(int port);
    int port() const;

    bool makeRequest(Method method,
                     const QString& uri,
                     const QByteArray& contentSend,
                     QByteArray* contentRecv = nullptr,
                     int timeout = HTTPCLENT_DEFAULT_TIMEOUT_MS);

    bool makeRequest(Method method,
                     const QString& uri,
                     const QJsonObject& jObjSend,
                     QJsonObject* jObjRecv = nullptr,
                     int timeout = HTTPCLENT_DEFAULT_TIMEOUT_MS);

signals:
    void messageError(const QString& error);

private:
    QNetworkRequest buildRequest(const QString& uri) const;
    QNetworkReply* execMethod(Method method,
                              QNetworkAccessManager& manager,
                              const QNetworkRequest& request,
                              const QByteArray& content) const;

private:
    QString m_hostName;
    int     m_port;

};
