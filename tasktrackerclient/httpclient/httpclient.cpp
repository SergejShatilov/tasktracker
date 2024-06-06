
#include "httpclient.h"

#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonArray>
#include <QEventLoop>

#include "dialogs/dialogauthorization/dialogauthorization.h"

// =============================================================================
HttpClient::HttpClient(QObject *parent) :
    QObject(parent),
    m_lastErrorString(""),
    m_hostName("127.0.0.1"),
    m_port(80),
    m_dataBaseName("undefined"),
    m_isAuthorized(false)
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
    return del(
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
void HttpClient::showError()
{
    QMessageBox message(QMessageBox::Critical,
                        QObject::tr("Error"),
                        m_lastErrorString,
                        QMessageBox::Ok);
    message.exec();
}

// =============================================================================
bool HttpClient::checkAuthorization()
{
    if (m_isAuthorized)
        return true;

    DialogAuthorization dialog;
    if (dialog.exec() != QDialog::Accepted)
        return false;

    m_userName = dialog.userName();
    m_password = dialog.password();
    return true;
}

// =============================================================================
bool HttpClient::post(const QString& endPoint, const QJsonObject& jObj)
{
    bool result = false;

    if (!checkAuthorization())
        return false;

    QNetworkAccessManager manager(this);
    connect(&manager, &QNetworkAccessManager::finished,
            this, [this, &result](QNetworkReply* reply)
    {
        if (reply->error() == QNetworkReply::NetworkError::NoError) {
            m_isAuthorized = true;
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

    const QString authorization = QString("%1, %2").arg(m_userName).arg(m_password);

    const QUrl url = QUrl(QString("http://%1:%2%3").arg(m_hostName).arg(m_port).arg(endPoint));
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", authorization.toLocal8Bit());
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
bool HttpClient::get(const QString& endPoint, QJsonObject& jObj)
{
    bool result = false;

    if (!checkAuthorization())
        return false;

    QNetworkAccessManager manager(this);
    connect(&manager, &QNetworkAccessManager::finished,
            this, [this, &result, &jObj](QNetworkReply* reply)
    {
        if (reply->error() == QNetworkReply::NetworkError::NoError) {
            jObj = QJsonDocument::fromJson(reply->readAll()).object();
            m_isAuthorized = true;
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

    const QString authorization = QString("%1, %2").arg(m_userName).arg(m_password);

    const QUrl url = QUrl(QString("http://%1:%2%3").arg(m_hostName).arg(m_port).arg(endPoint));
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", authorization.toLocal8Bit());
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
bool HttpClient::del(const QString& endPoint)
{
    bool result = false;

    if (!checkAuthorization())
        return false;

    QNetworkAccessManager manager(this);
    connect(&manager, &QNetworkAccessManager::finished,
            this, [this, &result](QNetworkReply* reply)
    {
        if (reply->error() == QNetworkReply::NetworkError::NoError) {
            m_isAuthorized = true;
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

    const QString authorization = QString("%1, %2").arg(m_userName).arg(m_password);

    const QUrl url = QUrl(QString("http://%1:%2%3").arg(m_hostName).arg(m_port).arg(endPoint));
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", authorization.toLocal8Bit());
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
