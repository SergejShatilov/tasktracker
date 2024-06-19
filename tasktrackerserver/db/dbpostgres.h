
#include <QSqlDatabase>

#include "dbmanager.h"

class DbPostgres : public DbManager
{
public:
    explicit DbPostgres(const QString& hostName, int port);

    QString dbType() const override;

    void createDb(const QString& dbName) override;

    void checkExistDb(const QString& dbName) override;

    void createEmployee(const QString& dbName,
                        QSharedPointer<Employee> employee) override;

    void deleteEmployee(const QString& dbName,
                        qint32 id) override;

    void changeEmployee(const QString& dbName,
                        const QSharedPointer<Employee> employee) override;

    QList<QSharedPointer<Employee>> getEmployees(
        const QString& dbName
    ) override;

    void createTask(const QString& dbName,
                    QSharedPointer<Task> task) override;
    void deleteTask(const QString& dbName,
                    qint32 id) override;
    void changeTask(const QString& dbName,
                    const QSharedPointer<Task> task) override;
    QList<QSharedPointer<Task>> getTasks(const QString& dbName) override;

private:
    QSqlDatabase addDatabase() const;
    QString m_hostName;
    int     m_port;
};
