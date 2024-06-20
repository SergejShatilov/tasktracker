
#include "expiredtasksdelegate.h"

#include <QHeaderView>

#include "models/tasksmodel.h"

// =============================================================================
ExpiredTasksDelegate::ExpiredTasksDelegate(TasksModel* tasksModel,
                                           QObject* parent) :
    QStyledItemDelegate(parent),
    m_tasksModel(tasksModel),
    m_proxyModel(new ExpiredTasksProxyModel(this))
{
    m_proxyModel->setSourceModel(m_tasksModel);
}

// =============================================================================
QWidget* ExpiredTasksDelegate::createEditor(
    QWidget* parent,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QTreeView* treeView = new QTreeView(parent);
    treeView->setAutoFillBackground(true);
    treeView->setModel(m_proxyModel);

    // Скрываем все колонки, кроме ...
    hideAllColumnExcept(
        treeView,
        QStringList({
            "name",
            "id",
            "deadline"
        })
    );

    return treeView;
}

// =============================================================================
void ExpiredTasksDelegate::setEditorData(
    QWidget* editor,
    const QModelIndex& index) const
{
    Q_UNUSED(editor);
    m_proxyModel->setExecutorId(index.data(Qt::EditRole).toInt());
    auto treeView = static_cast<QTreeView*>(editor);
    treeView->expandAll();
}

// =============================================================================
void ExpiredTasksDelegate::updateEditorGeometry(
    QWidget* editor,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    Q_UNUSED(index);

    editor->setGeometry(option.rect.x(), option.rect.y(),
                        option.rect.width(), 200);
}

// =============================================================================
void ExpiredTasksDelegate::hideAllColumnExcept(
    QTreeView* treeView,
    const QStringList& listFields) const
{
    auto tasksModel = static_cast<TasksModel*>(m_proxyModel->sourceModel());

    for (int i = 0; i < tasksModel->columnCount(QModelIndex()); ++i)
    {
        auto field = tasksModel->fieldByColumn(i);
        if (!listFields.contains(field))
            treeView->setColumnHidden(i, true);
    }
}

// =============================================================================
