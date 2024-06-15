
#pragma once

#include <QTreeView>
#include "httpclient/httpclient.h"
#include "models/employeesmodel.h"
#include "models/tasksmodel.h"

class EmployeesView : public QTreeView
{
    Q_OBJECT

public:
    explicit EmployeesView(HttpClient* httpClient,
                           QWidget* parent = nullptr);

    EmployeesModel* employeesModel() const;

    void setTasksModel(TasksModel* model);

public slots:
    void slotUpdate();

private slots:
    void slotCreate();
    void slotDelete();
    void slotEdit(const QModelIndex& index);

    void slotDoubleClicked(const QModelIndex& index);
    void slotContextMenu(const QPoint& pos);

private:
    HttpClient* m_httpClient;
    EmployeesModel* m_employeesModel;
    TasksModel* m_tasksModel;

};
