
#pragma once

#include "viewer.h"
#include "db/dbremotemanager.h"
#include "delegates/statedelegate.h"

class TasksViewer : public Viewer
{
    Q_OBJECT

public:
    explicit TasksViewer(DbRemoteManager* dbManager,
                         QWidget *parent = nullptr);

    void setModel(QAbstractItemModel* model);

private:
    void createByIndex(const QModelIndex& index) override;
    void editByIndex(const QModelIndex& index) override;
    void removeByIndex(const QModelIndex& index) override;
    void customContextMenu(QMenu* menu, const QModelIndex& index) override;
    void itemDoubleClicked(const QModelIndex& index) override;

private:
    StateDelegate* m_stateDelegate;

};
