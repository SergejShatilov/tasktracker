
#pragma once

#include <QObject>
#include <QNetworkReply>
#include <QJsonObject>

#include "employee.h"
#include "task.h"

class HttpClient : public QObject
{
    Q_OBJECT

public:
    explicit HttpClient(QObject* parent = nullptr);

    void setHostName(const QString& hostName);
    const QString& hostName() const;
    void setPort(int port);
    int port() const;

    void setDbName(const QString& name);
    const QString& dbName() const;

    bool createDb(const QString& dbname);
    bool openDb(const QString& dbname);

    bool addEmployee(const Employee& employee);
    bool deleteEmployee(qint32 id);
    bool getEmployees(QList<Employee>& list);
    bool changeEmployee(qint32 id, const Employee& employee);

    bool addTask(const Task& task);
    bool getTasks(QList<Task>& list);
    bool deleteTask(qint32 id);

private:
    void showError();
    bool checkAuthorization();
    bool post(const QString& endPoint, const QJsonObject& jObj);
    bool get(const QString& endPoint, QJsonObject& jObj);
    bool put(const QString& endPoint, const QJsonObject& jObj);
    bool del(const QString& endPoint);

signals:
    void finished();

private:
    QString m_lastErrorString;
    QString m_hostName;
    int     m_port;
    QString m_dataBaseName;
    bool    m_isAuthorized;
    QString m_userName;
    QString m_password;
};
