
#pragma once

#include <QStyledItemDelegate>
#include <QTreeView>

#include "models/tasksmodel.h"
#include "models/expiredtasksproxymodel.h"

class ExpiredTasksDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ExpiredTasksDelegate(TasksModel* tasksModel,
                                  QObject* parent = nullptr);

    QWidget* createEditor(QWidget* parent,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override;

    void setEditorData(QWidget* editor,
                       const QModelIndex& index) const override;

    void updateEditorGeometry(QWidget* editor,
                              const QStyleOptionViewItem& option,
                              const QModelIndex& index) const override;

private:
    void hideAllColumnExcept(QTreeView* treeView,
                             const QStringList& listFields) const;

private:
    TasksModel*             m_tasksModel;
    ExpiredTasksProxyModel* m_proxyModel;

};
