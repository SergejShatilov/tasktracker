
#include <QSqlDatabase>

#include "db/dbmanager.h"

class DBManagerPostgres : public DBManager
{
public:
    explicit DBManagerPostgres(const QString& hostName, int port);

    QString dbtype() const override;

    void createNewDB(const DBAuthorization& authorization,
                     const QString& dbname,
                     const QString& dbowner) override;

    void checkExistDb(const DBAuthorization& authorization,
                      const QString& dbname) override;

    void createNewEmployee(const DBAuthorization& authorization,
                           const QString& dbname,
                           const Employee& employee) override;

    void deleteEmployee(const DBAuthorization& authorization,
                        const QString& dbname,
                        qint32 id) override;

    QList<Employee> getEmployees(const DBAuthorization& authorization,
                                 const QString& dbname) override;

    void changeEmployee(const DBAuthorization& authorization,
                        const QString& dbname,
                        const Employee& employee) override;

private:
    QSqlDatabase addDatabase(const DBAuthorization& authorization) const;
};
