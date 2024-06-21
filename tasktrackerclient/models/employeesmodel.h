
#pragma once

#include "objectsmodel.h"
#include "db/dbremotemanager.h"

class EmployeesModel : public ObjectsModel
{
    Q_OBJECT

public:
    explicit EmployeesModel(DbRemoteManager* dbManager,
                            QObject* parent = nullptr);

public:
    Qt::ItemFlags flags(const QModelIndex &index) const override;

protected:
    virtual QVariant dataDisplayRole(const QModelIndex &index,
                                     QObject* obj,
                                     const QString& field) const override;
    virtual QVariant dataEditRole(const QModelIndex &index,
                                  QObject* obj,
                                  const QString& field) const override;
    virtual QVariant dataBackgroundRole(const QModelIndex &index,
                                        QObject* obj,
                                        const QString& field) const override;
    virtual QVariant dataForegroundRole(const QModelIndex &index,
                                        QObject* obj,
                                        const QString& field) const override;

private slots:
    void addEmployee(Employee* employee);
    void removeEmployee(Employee* employee);
    void updateEmployees(const QList<Employee*>& listEmployees);

};
