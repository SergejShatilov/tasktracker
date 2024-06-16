
#pragma once

#include <QNetworkReply>
#include <QJsonObject>

#include "employee.h"
#include "task.h"

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

    void setDbName(const QString& dbName);
    const QString& dbName() const;

    bool createDb(const QString& dbName);
    bool openDb(const QString& dbName);

    Employee addEmployee(const Employee& employee);
    bool deleteEmployee(qint32 id);
    bool getEmployees(QList<Employee>& list);
    bool changeEmployee(qint32 id, const Employee& employee);

    Task addTask(const Task& task);
    bool deleteTask(qint32 id);
    bool getTasks(QList<Task>& list);
    bool changeTask(qint32 id, const Task& task);

    QByteArray makeRequest(Method method,
                           const QString& uri,
                           const QByteArray& content,
                           bool* result = nullptr);

    QByteArray makeRequest(Method method,
                           const QString& uri,
                           const QJsonObject& jObj,
                           bool* result = nullptr);

private:
    QString m_hostName;
    int     m_port;
    QString m_dataBaseName;
};
