
#pragma once

#include "objectsmodel.h"
#include "db/dbremotemanager.h"

class TasksModel : public ObjectsModel
{
    Q_OBJECT

public:
    explicit TasksModel(DbRemoteManager* dbManager,
                        QObject* parent = nullptr);

public:
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index,
                 const QVariant &value,
                 int role) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private slots:
    void addTask(Task* task);
    void removeTask(Task* task, bool isExtract);
    void updateTasks(QList<Task*> listTasks);

};
