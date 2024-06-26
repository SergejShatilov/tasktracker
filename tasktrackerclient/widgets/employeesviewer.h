
#pragma once

#include "viewer.h"
#include "db/dbremotemanager.h"

class EmployeesViewer : public Viewer
{
    Q_OBJECT

public:
    explicit EmployeesViewer(DbRemoteManager* dbManager,
                             QWidget *parent = nullptr);

    void setExpiredTasksDelegate(QAbstractItemDelegate* delegate);

private:
    void createByIndex(const QModelIndex& index) override;
    void editByIndex(const QModelIndex& index) override;
    void removeByIndex(const QModelIndex& index) override;
    void customContextMenu(QMenu* menu, const QModelIndex& index) override;
    void itemDoubleClicked(const QModelIndex& index) override;

};
