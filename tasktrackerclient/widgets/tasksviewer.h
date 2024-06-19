
#pragma once

#include "viewer.h"
#include "db/dbremotemanager.h"

class TasksViewer : public Viewer
{
    Q_OBJECT

public:
    explicit TasksViewer(DbRemoteManager* dbManager,
                         QWidget *parent = nullptr);

private:
    void createByIndex(const QModelIndex& index) override;
    void editByIndex(const QModelIndex& index) override;
    void removeByIndex(const QModelIndex& index) override;
    void customContextMenu(QMenu* menu, const QModelIndex& index) override;
    void itemDoubleClicked(const QModelIndex& index) override;

};
