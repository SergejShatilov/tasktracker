
#pragma once

#include <QTreeView>
#include "httpclient/httpclient.h"
#include "models/tasksmodel.h"

class TasksView : public QTreeView
{
    Q_OBJECT

public:
    explicit TasksView(std::shared_ptr<HttpClient> httpClient,
                       QWidget* parent = nullptr);

public slots:
    void slotUpdate();

private slots:
    void slotCreate();
    void slotDelete();
    void slotEdit(const QModelIndex& index);

    void slotDoubleClicked(const QModelIndex& index);
    void slotContextMenu(const QPoint& pos);

private:
    std::shared_ptr<HttpClient> m_httpClient;
    TasksModel* m_tasksModel;

};
