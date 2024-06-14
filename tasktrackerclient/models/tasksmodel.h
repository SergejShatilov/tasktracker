
#pragma once

#include <QAbstractItemModel>
#include "taskobject.h"
#include "employeesmodel.h"
#include "httpclient/httpclient.h"

class TasksModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TasksModel(HttpClient* httpClient,
                        QObject* parent = nullptr);
    ~TasksModel();

    void setEmployeesModel(EmployeesModel* model);

    void addTask(const Task& task,
                 const QModelIndex& parentIndex = QModelIndex());
    QList<qint32> removeTask(const QModelIndex& index);

    void loadFromList(const QList<Task>& list);

    QObject* findById(qint32 id) const;
    QModelIndex indexOf(QObject* obj) const;
    qint32 idByIndex(const QModelIndex& index) const;
    QString nameByIndex(const QModelIndex& index) const;
    TaskObject* taskObjectByIndex(const QModelIndex& index) const;

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
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    void clear();

protected:
    HttpClient*        m_httpClient;
    QStringList        m_columns;
    TaskObject*        m_rootItem;
    QList<TaskObject*> m_listObjects;
    EmployeesModel*    m_employeesModel;

};
