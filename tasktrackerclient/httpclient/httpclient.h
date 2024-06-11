
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

    Employee addEmployee(const Employee& employee);
    bool deleteEmployee(qint32 id);
    bool getEmployees(QList<Employee>& list);
    bool changeEmployee(qint32 id, const Employee& employee);

    Task addTask(const Task& task);
    bool deleteTask(qint32 id);
    bool getTasks(QList<Task>& list);
    bool changeTask(qint32 id, const Task& task);

private:
    void showError();
    bool post(const QString& endPoint, const QJsonObject& jObj);
    QByteArray post2(const QString& uri, const QByteArray& data);
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
};
