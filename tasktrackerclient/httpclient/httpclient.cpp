
#include "httpclient.h"

#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonArray>
#include <QEventLoop>

// =============================================================================
HttpClient::HttpClient(QObject *parent) :
    QObject(parent),
    m_lastErrorString(""),
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
void HttpClient::setDbName(const QString& name) {
    m_dataBaseName = name;
}

const QString& HttpClient::dbName() const {
    return m_dataBaseName;
}

// =============================================================================
bool HttpClient::createDb(const QString &dbname)
{
    QJsonObject jobj;
    jobj["dbname"] = dbname;

    return post("/", jobj);
}

// =============================================================================
bool HttpClient::openDb(const QString &dbname)
{
    QJsonObject jObj;
    return get(QString("/dbname-%1/").arg(dbname), jObj);
}

// =============================================================================
bool HttpClient::addEmployee(const Employee &employee)
{
    return post(QString("/dbname-%1/employees/").arg(m_dataBaseName),
                employee.toJsonObject());
}

// =============================================================================
bool HttpClient::deleteEmployee(qint32 id)
{
    return del
    (
        QString("/dbname-%1/employees/id-%2/")
        .arg(m_dataBaseName)
        .arg(id)
    );
}

// =============================================================================
bool HttpClient::getEmployees(QList<Employee>& list)
{
    QJsonObject jObj;
    if (!get(QString("/dbname-%1/employees/").arg(m_dataBaseName), jObj))
        return false;

    list.clear();

    QJsonArray jarray = jObj["employees"].toArray();
    for (const auto& jemployee : jarray) {
        list.append(Employee::fromJsonObject(jemployee.toObject()));
    }

    return true;
}

// =============================================================================
bool HttpClient::changeEmployee(qint32 id, const Employee& employee)
{
    return put
    (
        QString("/dbname-%1/employees/id-%2/")
            .arg(m_dataBaseName)
            .arg(id),
        employee.toJsonObject()
    );
}

// =============================================================================
Task HttpClient::addTask(const Task& task)
{
    const auto data = post2
    (
        QString("/dbname-%1/tasks/")
            .arg(m_dataBaseName),
        task.toJson()
    );

    return Task::fromJson(data);
}

// =============================================================================
bool HttpClient::getTasks(QList<Task>& list)
{
    QJsonObject jObj;
    if (!get(QString("/dbname-%1/tasks/").arg(m_dataBaseName), jObj))
        return false;

    list.clear();

    QJsonArray jarray = jObj["tasks"].toArray();
    for (const auto& jtask : jarray) {
        list.append(Task::fromJsonObject(jtask.toObject()));
    }

    return true;
}

// =============================================================================
bool HttpClient::deleteTask(qint32 id)
{
    return del
    (
        QString("/dbname-%1/tasks/id-%2/")
        .arg(m_dataBaseName)
        .arg(id)
    );
}

// =============================================================================
bool HttpClient::changeTask(qint32 id, const Task& task)
{
    return put
    (
        QString("/dbname-%1/tasks/id-%2/")
            .arg(m_dataBaseName)
            .arg(id),
        task.toJsonObject()
    );
}

// =============================================================================
void HttpClient::showError()
{
    QMessageBox message(QMessageBox::Critical,
                        QObject::tr("Error"),
                        m_lastErrorString,
                        QMessageBox::Ok);
    message.exec();
}

// =============================================================================
bool HttpClient::post(const QString& endPoint, const QJsonObject& jObj)
{
    bool result = false;

    QNetworkAccessManager manager(this);
    connect(&manager, &QNetworkAccessManager::finished,
            this, [this, &result](QNetworkReply* reply)
    {
        if (reply->error() == QNetworkReply::NetworkError::NoError) {
            result = true;
        }
        else
        {
            m_lastErrorString = QString("%1\r\n%2")
                .arg(reply->errorString())
                .arg(QString(reply->readAll()));
            showError();
        }

        emit finished();
    });

    const QUrl url = QUrl(QString("http://%1:%2%3").arg(m_hostName).arg(m_port).arg(endPoint));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, QString("TaskTrackerClient"));
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, QString("application/json"));

    manager.post(request, QJsonDocument(jObj).toJson(QJsonDocument::JsonFormat::Compact));

    // Ждем ответ
    QEventLoop eventLoop;
    connect(this, &HttpClient::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    return result;
}

// =============================================================================
QByteArray HttpClient::post2(const QString& uri, const QByteArray& data)
{
    QByteArray dataResult;

    QNetworkAccessManager manager(this);
    connect(&manager, &QNetworkAccessManager::finished,
            this, [this, &dataResult](QNetworkReply* reply)
    {
        if (reply->error() == QNetworkReply::NetworkError::NoError) {
            dataResult = reply->readAll();
            qDebug() << dataResult;
        }
        else
        {
            m_lastErrorString = QString("%1\r\n%2")
                .arg(reply->errorString())
                .arg(QString(reply->readAll()));
            showError();
        }

        emit finished();
    });

    const QUrl url = QUrl(QString("http://%1:%2%3").arg(m_hostName).arg(m_port).arg(uri));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, QString("TaskTrackerClient"));
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, QString("application/json"));

    manager.post(request, data);

    // Ждем ответ
    QEventLoop eventLoop;
    connect(this, &HttpClient::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    return dataResult;
}

// =============================================================================
bool HttpClient::get(const QString& endPoint, QJsonObject& jObj)
{
    bool result = false;

    QNetworkAccessManager manager(this);
    connect(&manager, &QNetworkAccessManager::finished,
            this, [this, &result, &jObj](QNetworkReply* reply)
    {
        if (reply->error() == QNetworkReply::NetworkError::NoError) {
            jObj = QJsonDocument::fromJson(reply->readAll()).object();
            result = true;
        }
        else
        {
            m_lastErrorString = QString("%1\r\n%2")
                .arg(reply->errorString())
                .arg(QString(reply->readAll()));
            showError();
        }

        emit finished();
    });

    const QUrl url = QUrl(QString("http://%1:%2%3").arg(m_hostName).arg(m_port).arg(endPoint));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, QString("TaskTrackerClient"));
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, QString("application/json"));

    manager.get(request);

    // Ждем ответ
    QEventLoop eventLoop;
    connect(this, &HttpClient::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    return result;
}

// =============================================================================
bool HttpClient::put(const QString& endPoint, const QJsonObject& jObj)
{
    bool result = false;

    QNetworkAccessManager manager(this);
    connect(&manager, &QNetworkAccessManager::finished,
            this, [this, &result](QNetworkReply* reply)
    {
        if (reply->error() == QNetworkReply::NetworkError::NoError) {
            result = true;
        }
        else
        {
            m_lastErrorString = QString("%1\r\n%2")
                .arg(reply->errorString())
                .arg(QString(reply->readAll()));
            showError();
        }

        emit finished();
    });

    const QUrl url = QUrl(QString("http://%1:%2%3").arg(m_hostName).arg(m_port).arg(endPoint));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, QString("TaskTrackerClient"));
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, QString("application/json"));

    manager.put(request, QJsonDocument(jObj).toJson(QJsonDocument::JsonFormat::Compact));

    // Ждем ответ
    QEventLoop eventLoop;
    connect(this, &HttpClient::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    return result;
}

// =============================================================================
bool HttpClient::del(const QString& endPoint)
{
    bool result = false;

    QNetworkAccessManager manager(this);
    connect(&manager, &QNetworkAccessManager::finished,
            this, [this, &result](QNetworkReply* reply)
    {
        if (reply->error() == QNetworkReply::NetworkError::NoError) {
            result = true;
        }
        else
        {
            m_lastErrorString = QString("%1\r\n%2")
                .arg(reply->errorString())
                .arg(QString(reply->readAll()));
            showError();
        }

        emit finished();
    });

    const QUrl url = QUrl(QString("http://%1:%2%3").arg(m_hostName).arg(m_port).arg(endPoint));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, QString("TaskTrackerClient"));
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, QString("application/json"));

    manager.deleteResource(request);

    // Ждем ответ
    QEventLoop eventLoop;
    connect(this, &HttpClient::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    return result;
}

// =============================================================================
