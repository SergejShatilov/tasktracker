
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
    bool setData(const QModelIndex &index,
                 const QVariant &value,
                 int role) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

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
    void addTask(Task* task);
    void removeTask(Task* task, bool isExtract);
    void updateTasks(QList<Task*> listTasks);

};
