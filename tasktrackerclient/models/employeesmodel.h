
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
    QVariant data(const QModelIndex &index, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

private slots:
    void addEmployee(Employee* employee);
    void removeEmployee(Employee* employee);
    void updateEmployees(const QList<Employee*>& listEmployees);

};
