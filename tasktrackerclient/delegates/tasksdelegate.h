
#pragma once

#include <QStyledItemDelegate>
#include "models/tasksmodel.h"
#include "models/taskexecutorfiltermodel.h"

class TasksDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit TasksDelegate(TasksModel* tasksModel,
                           QObject* parent = nullptr);

    QWidget* createEditor(QWidget* parent,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override;

    void setEditorData(QWidget* editor,
                       const QModelIndex& index) const override;

    void setModelData(QWidget* editor,
                      QAbstractItemModel* model,
                      const QModelIndex& index) const override;

    void updateEditorGeometry(QWidget* editor,
                              const QStyleOptionViewItem& option,
                              const QModelIndex& index) const override;

    void paint(QPainter* painter,
               const QStyleOptionViewItem &option,
               const QModelIndex& index) const override;

private:
    TasksModel* m_tasksModel;
    TaskExecutorFilterModel* m_proxyModel;

};
