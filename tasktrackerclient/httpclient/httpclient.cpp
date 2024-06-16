
#include "httpclient.h"

#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonArray>
#include <QEventLoop>
#include <QTimer>

// =============================================================================
HttpClient::HttpClient(QObject *parent) :
    QObject(parent),
    m_hostName("127.0.0.1"),
    m_port(80),
    m_dataBaseName("undefined")
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
void HttpClient::setDbName(const QString& dbName) {
    m_dataBaseName = dbName;
}

const QString& HttpClient::dbName() const {
    return m_dataBaseName;
}

// =============================================================================
bool HttpClient::createDb(const QString &dbName)
{
    QJsonObject jObj;
    jObj["dbName"] = dbName;

    bool result = false;
    QByteArray recvBuf = makeRequest(
        Method::Post,
        "/",
        jObj,
        &result
    );

    return result;
}

// =============================================================================
bool HttpClient::openDb(const QString &dbName)
{
    bool result = false;
    makeRequest(
        Method::Get,
        QString("/%1").arg(dbName),
        QByteArray(),
        &result
    );

    return result;
}

// =============================================================================
Employee HttpClient::addEmployee(const Employee &employee)
{
    bool result = false;
    QByteArray recvBuf = makeRequest(
        Method::Post,
        QString("/%1/employees").arg(m_dataBaseName),
        employee.toJson(),
        &result
    );

    if (!result)
        return Employee();

    return Employee::fromJson(recvBuf);
}

// =============================================================================
bool HttpClient::deleteEmployee(qint32 id)
{
    bool result = false;
    QByteArray recvBuf = makeRequest(
        Method::Delete,
        QString("/%1/employees/%2").arg(m_dataBaseName).arg(id),
        QByteArray(),
        &result
    );

    return result;
}

// =============================================================================
bool HttpClient::getEmployees(QList<Employee>& list)
{
    bool result = false;
    QByteArray recvBuf = makeRequest(
        Method::Get,
        QString("/%1/employees").arg(m_dataBaseName),
        QByteArray(),
        &result
    );

    if (!result)
        return false;

    list = Employee::listFromJson(recvBuf);

    return true;
}

// =============================================================================
bool HttpClient::changeEmployee(qint32 id, const Employee& employee)
{
    bool result = false;
    QByteArray recvBuf = makeRequest(
        Method::Put,
        QString("/%1/employees/%2").arg(m_dataBaseName).arg(id),
        employee.toJson(),
        &result
    );

    return result;
}

// =============================================================================
Task HttpClient::addTask(const Task& task)
{
    bool result = false;
    QByteArray recvBuf = makeRequest(
        Method::Post,
        QString("/%1/tasks").arg(m_dataBaseName),
        task.toJson(),
        &result
    );

    if (!result)
        return Task();

    return Task::fromJson(recvBuf);
}

// =============================================================================
bool HttpClient::deleteTask(qint32 id)
{
    bool result = false;
    QByteArray recvBuf = makeRequest(
        Method::Delete,
        QString("/%1/tasks/%2").arg(m_dataBaseName).arg(id),
        QByteArray(),
        &result
    );

    return result;
}

// =============================================================================
bool HttpClient::getTasks(QList<Task>& list)
{
    bool result = false;
    QByteArray recvBuf = makeRequest(
        Method::Get,
        QString("/%1/tasks").arg(m_dataBaseName),
        QByteArray(),
        &result
    );

    if (!result)
        return false;

    list = Task::listFromJson(recvBuf);

    return true;
}

// =============================================================================
bool HttpClient::changeTask(qint32 id, const Task& task)
{
    bool result = false;
    QByteArray recvBuf = makeRequest(
        Method::Put,
        QString("/%1/tasks/%2").arg(m_dataBaseName).arg(id),
        task.toJson(),
        &result
    );

    return result;
}

// =============================================================================
QByteArray HttpClient::makeRequest(Method method,
                                   const QString& uri,
                                   const QByteArray& content,
                                   bool* result)
{
    QTimer timer;
    timer.setInterval(5000);
    timer.setSingleShot(true);

    QEventLoop eventLoop;
    QNetworkAccessManager manager(this);

    // Формируем запрос
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

    // Делаем запрос
    QNetworkReply* reply = nullptr;
    switch (method)
    {
        case Method::Get:
            reply = manager.get(request);
            break;
        case Method::Post:
            reply = manager.post(request, content);
            break;
        case Method::Put:
            reply = manager.put(request, content);
            break;
        case Method::Delete:
            reply = manager.deleteResource(request);
            break;
        default:
        {
            if (result != nullptr)
                (*result) = false;

            return QByteArray("Error: [HttpClent] request method invalid");
        }
    }

    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, reply, &QNetworkReply::abort);

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

        QMessageBox message(QMessageBox::Critical,
                            tr("Error"),
                            error,
                            QMessageBox::Ok);
        message.exec();
    }

    reply->deleteLater();

    if (result != nullptr)
        (*result) = res;

    return recvBuf;
}

// =============================================================================
QByteArray HttpClient::makeRequest(Method method,
                                   const QString& uri,
                                   const QJsonObject& jObj,
                                   bool* result)
{
    QJsonDocument jDoc(jObj);

    return makeRequest(
        method,
        uri,
        jDoc.toJson(QJsonDocument::Compact),
        result
    );
}

// =============================================================================
