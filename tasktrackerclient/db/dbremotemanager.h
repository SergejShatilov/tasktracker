
/*
 * Менеджер для работы с удаленной базой данных
 */

#pragma once

#include "httpclient/httpclient.h"
#include "employee.h"
#include "task.h"

class DbRemoteManager : public HttpClient
{
    Q_OBJECT

public:
    explicit DbRemoteManager(QObject* parent = nullptr);

    void setDbName(const QString& dbName);
    const QString& dbName() const;

    bool createDb(const QString& dbName);
    bool checkExistDb(const QString& dbName);

    bool addEmployee(Employee* employee);
    bool changeEmployee(const Employee* employee);
    bool removeEmployee(Employee* employee);

    bool addTask(Task* task);
    bool changeTask(const Task* task);
    bool removeTask(Task* task, bool isExtract = false);

    void update();

    const QList<Employee*> employees() const;

signals:
    void addedEmployee(Employee* employee);
    void removedEmployee(Employee* employee);
    void updateEmployees(const QList<Employee*>& listEmployees);

    void addedTask(Task* task);
    void removedTask(Task* task, bool isExtract);
    void updateTasks(QList<Task*> listTasks);

private slots:
    void showError(const QString& error);

private:
    bool getEmployees();
    bool getTasks(QList<Task*>& listTasksTop);

private:
    QString          m_dbName;
    QList<Employee*> m_listEmployees;
    QList<Task*>     m_listTasks;
};
