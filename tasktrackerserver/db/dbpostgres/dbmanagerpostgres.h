
#include <QSqlDatabase>

#include "db/dbmanager.h"

class DBManagerPostgres : public DBManager
{
public:
    explicit DBManagerPostgres(const QString& hostName, int port);

    QString dbtype() const override;

    void createNewDB(const QString& dbname) override;

    void checkExistDb(const QString& dbname) override;

    void createNewEmployee(const QString& dbname,
                           const Employee& employee) override;

    void deleteEmployee(const QString& dbname,
                        qint32 id) override;

    QList<Employee> getEmployees(const QString& dbname) override;

    void changeEmployee(const QString& dbname,
                        const Employee& employee) override;

    void createNewTask(const QString& dbname,
                       const Task& task) override;

    QList<Task> getTasks(const QString& dbname) override;

    void deleteTask(const QString& dbname,
                    qint32 id) override;

    void changeTask(const QString& dbname,
                    const Task& task) override;

private:
    QSqlDatabase addDatabase() const;
};
