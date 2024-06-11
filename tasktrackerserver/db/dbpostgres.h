
#include <QSqlDatabase>

#include "dbmanager.h"

class DBPostgres : public DBManager
{
public:
    explicit DBPostgres(const QString& hostName, int port);

    QString dbtype() const override;

    // -------------------------------------------------------------------------
    void createNewDB(const QString& dbname) override;

    void checkExistDb(const QString& dbname) override;

    // -------------------------------------------------------------------------
    Employee createEmployee(const QString& dbname,
                            const Employee& employee) override;

    void deleteEmployee(const QString& dbname,
                        qint32 id) override;

    QList<Employee> getEmployees(const QString& dbname) override;

    void changeEmployee(const QString& dbname,
                        const Employee& employee) override;

    // -------------------------------------------------------------------------
    Task createTask(const QString& dbname,
                    const Task& task) override;

    void deleteTask(const QString& dbname,
                    qint32 id) override;

    QList<Task> getTasks(const QString& dbname) override;

    void changeTask(const QString& dbname,
                    const Task& task) override;

private:
    QSqlDatabase addDatabase() const;
    QString m_hostName;
    int     m_port;
};
