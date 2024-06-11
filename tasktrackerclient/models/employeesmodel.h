
#pragma once

#include <QAbstractItemModel>
#include "employeeobject.h"

class EmployeesModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit EmployeesModel(QObject* parent = nullptr);
    ~EmployeesModel();

    void addEmployee(const Employee& employee);
    void removeEmployee(const QModelIndex& index);

    void loadFromList(const QList<Employee>& list);

    qint32 idByIndex(const QModelIndex& index) const;
    qint32 idByFullName(const QString& fullName) const;

public:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index,
                 const QVariant &value,
                 int role) override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role) const override;

private:
    void clear();
    EmployeeObject* employeeObjectByIndex(const QModelIndex& index) const;

protected:
    QStringList     m_columns;
    EmployeeObject* m_rootItem;

};
