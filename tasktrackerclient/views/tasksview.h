
#pragma once

#include <QTreeView>
#include "httpclient/httpclient.h"
#include "models/tasksmodel.h"
#include "models/employeesmodel.h"

class TasksView : public QTreeView
{
    Q_OBJECT

public:
    explicit TasksView(HttpClient* httpClient,
                       QWidget* parent = nullptr);

    void setEmployeesModel(EmployeesModel* model);

public slots:
    void slotUpdate();

private slots:
    void slotCreate(const QModelIndex& index);
    void slotDelete();
    void slotEdit(const QModelIndex& index);

    void slotDoubleClicked(const QModelIndex& index);
    void slotContextMenu(const QPoint& pos);

private:
    HttpClient*     m_httpClient;
    TasksModel*     m_tasksModel;
    EmployeesModel* m_employeesModel;

};
