
#pragma once

#include <QAbstractItemModel>
#include "taskobject.h"

class TasksModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TasksModel(QObject* parent = nullptr);
    ~TasksModel();

    void addTask(const Task& employee);
    void removeTask(const QModelIndex& index);

    void loadFromList(const QList<Task>& list);

    qint32 idByIndex(const QModelIndex& index) const;

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
    TaskObject* taskObjectByIndex(const QModelIndex& index) const;

protected:
    QStringList     m_columns;
    TaskObject* m_rootItem;

};
