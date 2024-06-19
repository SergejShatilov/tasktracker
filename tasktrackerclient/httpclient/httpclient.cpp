
#include "httpclient.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QEventLoop>
#include <QTimer>

// =============================================================================
HttpClient::HttpClient(QObject *parent) :
    QObject(parent),
    m_hostName("127.0.0.1"),
    m_port(80)
{
}

// =============================================================================
void HttpClient::setHostName(const QString& hostName) {
    m_hostName = hostName;
}

const QString& HttpClient::hostName() const {
    return m_hostName;
}

// =============================================================================
void HttpClient::setPort(int port) {
    m_port = port;
}

int HttpClient::port() const {
    return m_port;
}

// =============================================================================
bool HttpClient::makeRequest(Method method,
                             const QString& uri,
                             const QByteArray& content,
                             QByteArray* contentRecv,
                             int timeout)
{
    QTimer timer;
    timer.setInterval(timeout);
    timer.setSingleShot(true);

    QEventLoop eventLoop;
    QNetworkAccessManager manager(this);

    QNetworkRequest request = buildRequest(uri);

    // Делаем запрос
    auto reply = QScopedPointer<QNetworkReply>(execMethod(
        method,
        manager,
        request,
        content
    ));

    connect(reply.get(), &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, reply.get(), &QNetworkReply::abort);

    // Ждем ответ
    timer.start();
    eventLoop.exec();

    // Обрабатываем ответ
    bool res = false;
    QByteArray recvBuf;
    if (reply->isFinished() && reply->error() == QNetworkReply::NoError)
    {
        recvBuf = reply->readAll();
        res = true;
    }
    else
    {
        QString error = QString("%1\r\n%2")
                        .arg(reply->errorString())
                        .arg(QString(reply->readAll()));
        emit messageError(error);
    }

    if (contentRecv != nullptr)
        (*contentRecv) = recvBuf;

    return res;
}

// =============================================================================
bool HttpClient::makeRequest(Method method,
                             const QString& uri,
                             const QJsonObject& jObjSend,
                             QJsonObject* jObjRecv,
                             int timeout)
{
    QByteArray bufRecv;
    QByteArray bufSend = (jObjSend.isEmpty()) ? QByteArray() :
                         QJsonDocument(jObjSend).toJson(QJsonDocument::Compact);

    bool res = makeRequest(
        method,
        uri,
        bufSend,
        &bufRecv,
        timeout
    );

    if (!res)
        return false;

    if (!bufRecv.isEmpty() && (jObjRecv != nullptr))
        (*jObjRecv) = QJsonDocument::fromJson(bufRecv).object();

    return true;
}

// =============================================================================
QNetworkRequest HttpClient::buildRequest(const QString& uri) const
{
    const QUrl url = QUrl(
        QString("http://%1:%2%3")
        .arg(m_hostName)
        .arg(m_port)
        .arg(uri)
    );

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      QString("HttpClient"));

    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      QString("application/json"));

    return request;
}

// =============================================================================
QNetworkReply* HttpClient::execMethod(Method method,
                                      QNetworkAccessManager& manager,
                                      const QNetworkRequest& request,
                                      const QByteArray& content) const
{
    switch (method)
    {
        case Method::Get:    return manager.get(request);
        case Method::Post:   return manager.post(request, content);
        case Method::Put:    return manager.put(request, content);
        case Method::Delete: return manager.deleteResource(request);
        default:
            throw std::runtime_error("Request method invalid");
    }
}

// =============================================================================
